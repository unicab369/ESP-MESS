
class Serv_Tweet {
    Serv_Device *device;

    public:
        Tweet_Sync tweetSync;
        Tweet_Command command;
        Tweet_Attendant attendant;
        Tweet_Record record;

        TweetRecordCb *tweetRecordCb;

        void addDisplayQueues(String str) {
            device->addDisplayQueues(str, 6);
        }

        std::function<void()> setLadderIdCb = [&]() {
            device->showLadderId();
        };

        void setup(Serv_Device *_device, uint8_t *mac, std::function<void(DataPacket2*)> *onTweet2) {
            attendant.reconfigure(mac, &setLadderIdCb);
            command.reconfigure(mac);
            tweetSync.setup(mac);
            record.setup(mac);
            command.sendStartup();
            device = _device;

            command.onTweet2 = onTweet2;
            tweetSync.onTweet2 = onTweet2;
            record.onTweet2 = onTweet2;
            attendant.onTweet2 = onTweet2;

            record.callback = tweetRecordCb;
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

        void handleMessage(ReceivePacket2* packet) {
            DataContent& content = packet->dataPacket.content;
            // device->handleReceivePacket(packet);

            switch (packet->dataPacket.info.sourceCmd) {
                case CMD_TRIGGER: {
                    device->handleCommandTrigger(packet); break;
                }
                case CMD_SYNC: {
                    addDisplayQueues("Recv CMD_SYNC: ");
                    tweetSync.handleMessage(&content.syncItem); break;
                }
                case CMD_POST: {
                    addDisplayQueues("Recv CMD_POST: ");
                    record.handleMessage(&content.recordItem); break;
                }
                case CMD_ATTENDANT: {
                    addDisplayQueues("Recv CMD_ATTENDANT: ");
                    attendant.handleMessage(packet); break;
                }
            }
        }
};