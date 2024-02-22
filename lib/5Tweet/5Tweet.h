// typedef std::function<void(DataPacket*)> DataPacketCb;

template <typename T, SourceCmd command>
class Interface_Tweet {
    protected:
        T item;
        // uint8_t myMac[6];

    public:    
        uint8_t myChannel;
        Interface_Net* interface;

        // std::function<void(DataPacket2*)> *onTweet2;

        void setup(Interface_Net* _interface) {
            interface = _interface;
        }

        // void setup(uint8_t *mac) {
        //     memcpy(myMac, mac, sizeof(myMac));
        // }

        void sendMessage2(uint8_t groupId = 0) {
            if (interface == nullptr) return;
            uint8_t data[32] = { 0 };
            memcpy(data, &item, sizeof(item));
            DataPacket2 packet = DataPacket2::make(&data, command, groupId);
            interface->sendData(&packet, sizeof(DataPacket2));       
        }
};

#include "Tweet_Sync.h"
#include "Tweet_Record.h"
#include "Tweet_Attendant.h"
#include "Tweet_Command.h"

