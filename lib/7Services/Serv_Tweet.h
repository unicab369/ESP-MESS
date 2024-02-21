
class Serv_Tweet {
    public:
        Tweet_Sync tweetSync;
        Tweet_Command command;
        Tweet_Attendant attendant;
        Tweet_Record record;

        std::function<void()> setLadderIdCb = [&]() {
            // device->showLadderId();
        };

        void setup(Interface_Net *interface) {
            attendant.setup(interface);
            command.setup(interface);
            tweetSync.setup(interface);
            record.setup(interface);
            command.sendStartup();
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