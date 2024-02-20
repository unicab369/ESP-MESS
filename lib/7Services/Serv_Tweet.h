
class Serv_Tweet {
    Interface_Device *interface;

    public:
        Tweet_Sync tweetSync;
        Tweet_Command command;
        Tweet_Attendant attendant;
        Tweet_Record record;

        TweetRecordCb *tweetRecordCb;

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

        void addDisplayQueues(String str) {
            interface->addDisplayQueues(str, 6);
        }

        void handleMessage(ReceivePacket2* packet) {
            DataContent content = packet->dataPacket.content;

            switch (packet->dataPacket.info.sourceCmd) {
                case CMD_TRIGGER: {
                    interface->handlePacket(packet); break;
                }
                case CMD_SYNC: {
                    addDisplayQueues("Recv CMD_SYNC: ");
                    tweetSync.handleMessage(&content.syncItem); break;
                }
                case CMD_POST: {
                    addDisplayQueues("Recv CMD_POST: ");
                    interface->addPlotterQueue(content);
                }
                case CMD_ATTENDANT: {
                    addDisplayQueues("Recv CMD_ATTENDANT: ");
                    attendant.handleMessage(packet); break;
                }
            }
        }
};