#include "1Controller.h"
#include "3Sensor.h"
#include "2Storage.h"
#include "4Display.h"

enum SourceCmd: uint8_t {
    // CMD_SENSORS = 0x10,
    CMD_POST = 0xF0,
    CMD_SYNC = 0xF1,
    CMD_ATTENDANT = 0xF2,
    CMD_TRIGGER = 0xF3,
    CMD_DEFAULT = 0xFF
};

struct PacketXfo {
    uint32_t timeStamp = 0;
    uint8_t groupId = 0;
    uint8_t sourceCmd = CMD_DEFAULT;
    uint16_t validationCode = 0xEEEE;
};

struct DataPacket {
    PacketXfo info;
    uint8_t data[32] = { 0 };

    void reset() {
        info.timeStamp = 0;
        info.sourceCmd = CMD_DEFAULT;
    }

    bool validate(DataPacket *packetData) {
        // if (packetData->timeStamp != timeStamp) { return false; }
        if (packetData->info.sourceCmd != info.sourceCmd) { return false; }
        // if (packetData->validationCode != validationCode) { return false; }
        return true;
    }

    static DataPacket make(void *bufData, SourceCmd sourceCmd, uint8_t groupId = 0) {
        DataPacket packet;
        packet.info.timeStamp = millis();
        packet.info.sourceCmd = sourceCmd;
        packet.info.groupId = groupId;
        memcpy(&packet.data, bufData, sizeof(data));
        return packet;
    }
};

struct ReceivePacket {
    uint8_t sourceB[6] = { 0 };
    DataPacket packetData;

    String getSource(bool shortStr = false) {
        char info[22];
        sprintf(info, shortStr ? "%02X%02X%02X%02X%02X%02X" : "%02X:%02X:%02X:%02X:%02X:%02X", 
                    sourceB[0], sourceB[1], sourceB[2], sourceB[3], sourceB[4], sourceB[5]);
        return info;
    }

    void reset() {
        packetData.reset();
        // source.reset();
    }

    void print() {
        Serial.print("timeStamp = "); Serial.println(packetData.info.timeStamp);
        // printBuffer("\n*Receiv_packet", this, sizeof(ReceivePacket));
    }

    // check if sources are the same and timeStamp different doesn't exceed timeWindow
    bool check(ReceivePacket *packet, uint32_t timeWindow = 500) {
        // if (source.isEqual(&packet->source) == false) { return false; }
        // if (packet->packetData.timeStamp - packetData.timeStamp < timeWindow) { return false; }
        memcpy(this, packet, sizeof(ReceivePacket));
        return true;
    }

    bool validate(ReceivePacket *packet) {
        // return packetData.timeStamp != 0;
        // if (source.isEqual(&packet->source) == false) { return false; }
        // if (packetData.validate(&packet->packetData) == false) { return false; }
        return true;
    }
    
    bool validateSender() {
        return std::all_of(sourceB, sourceB+6, [](uint8_t val) { return val == 0; }) == false;
    }

    void clearSender() {
        memcpy(sourceB, 0, sizeof(sourceB));
    }

    static ReceivePacket make(const uint8_t *sourceB, const uint8_t *data) {
        ReceivePacket output;
        memcpy(&output.packetData, data, sizeof(DataPacket));
        memcpy(&output.sourceB, sourceB, 6);
        return output;
    }
};

typedef std::function<void(ReceivePacket*)> ReceivePacketCb;

enum CMD_Sync: uint8_t {
    SYNC_MOCK,
    SYNC_BOUNCE
};

struct SyncItem {
    CMD_Sync cue;
    uint8_t srcChannel;
    uint8_t destChannel;
    uint32_t timeStamp;
    uint8_t source[6];

    void setSource(uint8_t* mac) {
        memcpy(source, mac, 6);
        timeStamp = millis();
    }

    bool checkSource(uint8_t* mac) {
        return (memcmp(source, mac, 6) == 0);    // return true if matched
    }

    uint32_t getTransmitTime() {
        return millis() - timeStamp;
    }
};

struct CommandItem {
    Cue_Trigger cue;
    uint32_t value;
    bool flag;

    void update(Cue_Trigger newCue, uint32_t newValue = 0, bool newFlag = false) {
        cue = newCue;
        value = newValue;
        flag = newFlag;
    }
};

enum Sign_Record: uint8_t {
    RECORD_TEMPHUMLUX,
    RECORD_USERDEFINED
};

struct RecordItem {
    Sign_Record cue;
    float value1, value2, value3;

    void update(float val1, float val2, float val3) {
        cue = RECORD_TEMPHUMLUX;
        value1 = val1;
        value2 = val2;
        value3 = val3;
    }
};

enum CMD_Attedant: uint8_t {
    MASTER_RECRUITING,
    SLAVE_CLAIM,
    SLAVE_PROPOSE,
    MASTER_ASSIGN,
    SLAVE_CONFIRM
};

class AttedantItem {
    public:
        CMD_Attedant cue;
        uint16_t next_ladderId;
        uint8_t target_mac[6];

        void recuitCommand() {
            cue = MASTER_RECRUITING;
            memset(target_mac, 0, sizeof(target_mac));
            next_ladderId++;
        }

        void setTarget(uint8_t* mac) {
            memcpy(target_mac, mac, 6);
        }

        bool checkTarget(uint8_t *mac) {
            return (memcmp(target_mac, mac, 6) == 0);    // return true if matched
        }

        DataPacket makePacket(CMD_Attedant _cmd) {
            cue = _cmd;
            uint8_t data[16] = { 0 };
            memcpy(data, this, sizeof(AttedantItem));
            DataPacket packet = DataPacket::make(&data, CMD_ATTENDANT);
            return packet;
        }
};

union DataContent {
    uint8_t data[32] = { 0 };
    CommandItem commandItem;
    SyncItem syncItem;
    RecordItem recordItem;
    AttedantItem attItem;
};

struct DataPacket2 {
    PacketXfo info;
    DataContent content;

    static DataPacket2 make(void *buff, SourceCmd sourceCmd, uint8_t groupId = 0) {
        DataPacket2 packet;
        packet.info.timeStamp = millis();
        packet.info.sourceCmd = sourceCmd;
        packet.info.groupId = groupId;
        memcpy(&packet.content, buff, sizeof(packet.content));
        return packet;
    }
};

struct ReceivePacket2 {
    uint8_t sourceB[6] = { 0 };
    DataPacket2 dataPacket;

    static ReceivePacket2 make(const uint8_t *sourceB, const uint8_t *data) {
        ReceivePacket2 output;
        memcpy(&output.sourceB, sourceB, 6);                                  
        memcpy(&output.dataPacket, data, sizeof(dataPacket));
        return output;
    }

    String getSource(bool shortStr = false) {
        char info[22];
        sprintf(info, shortStr ? "%02X%02X%02X%02X%02X%02X" : "%02X:%02X:%02X:%02X:%02X:%02X", 
                    sourceB[0], sourceB[1], sourceB[2], sourceB[3], sourceB[4], sourceB[5]);
        return info;
    }
    
    void printData() {
        AppPrintHex(&dataPacket, sizeof(DataPacket2));
    }
};

typedef std::function<void(DataPacket2*)> DataPacketCb2;
typedef std::function<void(ReceivePacket2*)> ReceivePacketCb2;

#include "6Network.h"

#include "5Tweet.h"

#include "Serv_I2C.h"
#include "Serv_ESP.h"
#include "Serv_Power.h"
#include "Serv_Serial.h"
#include "Serv_Device.h"
#include "Serv_Tweet.h"
#include "Serv_Network.h"
#include "Serv_Behaviors.h"
