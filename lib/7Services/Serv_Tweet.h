
class Serv_Tweet {
    Serv_Device *device;

    public:
        Tweet_Sync tweetSync;
        Tweet_Command command;
        Tweet_Attendant attendant;
        Tweet_Record record;

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

        void handleCommandItem(CommandItem* item) {
            switch (item->cue) {
                case TRIGGER_STARTUP: {
                    addDisplayQueues("Recv Startup");
                    break;
                }
                case TRIGGER_SINGLECLICK: {
                    addDisplayQueues("Recv Single: " + String(item->value));
                    // device->led.toggle();
                    break;
                }
                case TRIGGER_DOUBLECLICK: {
                    addDisplayQueues("Recv Double: " + String(item->value));
                    // device->led.repeatPulses(1000);
                    break;  
                }
                case TRIGGER_PIR: {
                    addDisplayQueues("Recv Pir: " + String(item->value));
                    break;
                }
                case TRIGGER_IR: {
                    addDisplayQueues("Recv Ir: " + String(item->value));
                    break;
                }
                default: {
                    addDisplayQueues("Recv Unknown");
                }
            }
        }

        void handleMessage(ReceivePacket2* packet) {
            DataContent& content = packet->dataPacket.content;

            switch (packet->dataPacket.info.sourceCmd) {
                case CMD_TRIGGER: {
                    handleCommandItem(&content.commandItem); break;
                }
                case CMD_SYNC: {
                    tweetSync.handleMessage(&content.syncItem); break;
                }
                case CMD_POST: {
                    record.handleMessage(&content.recordItem); break;
                }
                case CMD_ATTENDANT: {
                    attendant.handleMessage(packet); break;
                }
            }
        }
};