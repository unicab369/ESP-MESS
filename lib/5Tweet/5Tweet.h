typedef std::function<void(DataPacket*)> DataPacketCb;

template <typename T, SourceCmd command>
class Interface_Tweet {
    protected:
        T item;
        uint8_t myMac[6];

    public:    
        uint8_t myChannel;
        
        std::function<void(DataPacket2*)> *onTweet2;

        void setup(uint8_t *mac) {
            memcpy(myMac, mac, sizeof(myMac));
        }

        void sendMessage2(uint8_t groupId = 0) {
            if (onTweet2 == nullptr) return;
            uint8_t data[32] = { 0 };
            memcpy(data, &item, sizeof(item));
            DataPacket2 packet = DataPacket2::make(&data, command, groupId);
            (*onTweet2)(&packet);        
        }
};

#include "Tweet_Sync.h"
#include "Tweet_Record.h"
#include "Tweet_Attendant.h"
#include "Tweet_Command.h"

