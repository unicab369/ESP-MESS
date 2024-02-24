enum Type_Message: uint8_t {
   CMD_POST = 0xF0,
   CMD_SYNC = 0xF1,
   CMD_ATTENDANT = 0xF2,
   CMD_TRIGGER = 0xF3,
   CMD_DEFAULT = 0xFF
};

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

void appendParam(char* output, uint8_t* id, const char* key, float value) {
   sprintf(output + strlen(output), "\"%02x%02x%02x_%s\":[{\"value\":%.2f}]", id[3], id[4], id[5], key, value);
}

struct RecordItem {
   Sign_Record cue;
   float value1, value2, value3;
   float value4, value5;

   void update(float val1, float val2, float val3, float val4, float val5) {
      cue = RECORD_TEMPHUMLUX;
      value1 = val1;
      value2 = val2;
      value3 = val3;
      value4 = val4;
      value5 = val5;
   }

   void makeJson(char* output, uint8_t* id) {
      strcat(output, "{\"data\":{");
      appendParam(output, id, "Temp", value1); strcat(output, ",");
      appendParam(output, id, "Hum", value2); strcat(output, ",");
      appendParam(output, id, "Lux", value3); strcat(output, ",");
      appendParam(output, id, "Volt", value4); strcat(output, ",");
      appendParam(output, id, "mA", value5); 
      strcat(output, "}}");
   }

   void printData() {
      Loggable logger = Loggable("RecordItem");
      logger.xLogf("Data val1 = %f, val2 = %f, val3 = %f, val4 = %f", 
                     value1, value2, value3, value4); 
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

      // DataPacket makePacket(CMD_Attedant _cmd) {
      //    cue = _cmd;
      //    uint8_t data[16] = { 0 };
      //    memcpy(data, this, sizeof(AttedantItem));
      //    DataPacket packet = DataPacket::make(&data, CMD_ATTENDANT);
      //    return packet;
      // }
};

union DataContent {
   uint8_t data[32] = { 0 };
   CommandItem commandItem;
   SyncItem syncItem;
   RecordItem recordItem;
   AttedantItem attItem;
};

struct DataPacket2 {
   uint32_t timeStamp = 0;
   uint8_t groupId = 0;
   Type_Message msgType = CMD_DEFAULT;
   uint16_t validationCode = 0xEEEE;
   
   DataContent content;

   static DataPacket2 make(void *buff, Type_Message type, uint8_t groupId = 0) {
      DataPacket2 packet;
      packet.timeStamp = millis();
      packet.msgType = type;
      packet.groupId = groupId;
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

   const char* getSource(bool shortStr = false) {
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