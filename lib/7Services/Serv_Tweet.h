
class Serv_Tweet {
    Interface_Device *interface;

    public:
        Tweet_Sync tweetSync;
        Tweet_Command command;
        Tweet_Attendant attendant;
        Tweet_Record record;

        std::function<void()> setLadderIdCb = [&]() {
            // device->showLadderId();
        };

        void setup(Interface_Device *_interface, uint8_t *mac, std::function<void(DataPacket2*)> *onTweet2) {
            attendant.reconfigure(mac, &setLadderIdCb);
            command.reconfigure(mac);
            tweetSync.setup(mac);
            record.setup(mac);
            command.sendStartup();
            interface = _interface;

            command.onTweet2 = onTweet2;
            tweetSync.onTweet2 = onTweet2;
            record.onTweet2 = onTweet2;
            attendant.onTweet2 = onTweet2;
        } 

        void updateChannel(uint8_t channel) {
            command.myChannel = channel;
            tweetSync.myChannel = channel;
            record.myChannel = channel;
            attendant.myChannel = channel;
        }
        
        void sendSyncMock() {
            tweetSync.sendSyncMock();
        }
};