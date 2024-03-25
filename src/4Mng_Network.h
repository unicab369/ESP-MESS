
#include "7WebServer/Web_Server.h"      // Flash +6%

class Mng_Network {
    Web_Server wServer;
    Net_Radio radio;
    Net_Lora lora;
    Net_UDP udp;

    AppQueue<ReceivePacket2, MAX_PLOTTER_QUEUE> queuePlotter;

    std::function<void(time_t*)> udpTimeRequestCb = [&](time_t* time) {
        time_t _time = *time;
        device->updateTimer(_time);
    }; 

    public:
        Serv_Network servWifi;
        Serv_EspNow espNow;
        Serv_Tweet tweet;
        Serv_Device *device;

        const char* getHostName()   { return servWifi.wifi.hostName; }
        const char* getNetworkId()  { return servWifi.wifi.localIp(); }

        void setup(Serv_Device* _device) {
            device = _device;

            servWifi.onWifiConnected = [&](bool connected) {
                char *dateStr = servWifi.interface->getDateStr();
                AppPrint("\n[Runtime]", "configure storage path = " + String(dateStr));
                servWifi.interface->getStorage()->loadStoragePath(dateStr);
                AppPrintHeap();

                espNow.setup(WiFi.channel());

                espNow.callback = [&](ReceivePacket2* packet) {
                    DataContent content = packet->dataPacket.content;

                    switch (packet->dataPacket.msgType) {
                        case CMD_TRIGGER: {
                            CommandItem item = packet->dataPacket.content.commandItem;

                            switch (item.cue) {
                                case TRIGGER_STARTUP: {
                                    device->addDisplayQueues("Recv Startup", 6);
                                    break;
                                }
                                case TRIGGER_SINGLECLICK: {
                                    char output[22];
                                    sprintf(output, "Recv Single: %lu", item.value);
                                    device->addDisplayQueues(output, 6);
                                    // device->led.toggle();
                                    break;
                                }
                                case TRIGGER_DOUBLECLICK: {
                                    char output[22];
                                    sprintf(output, "Recv Double: %lu", item.value);
                                    device->addDisplayQueues(output, 6);
                                    // device->led.repeatPulses(1000);
                                    break;  
                                }
                                case TRIGGER_PIR: {
                                    char output[22];
                                    sprintf(output, "Recv Pir: %lu", item.value);
                                    device->addDisplayQueues(output, 6);
                                    break;
                                }
                                case TRIGGER_IR: {
                                    char output[22];
                                    sprintf(output, "Recv Ir: %lu", item.value);
                                    device->addDisplayQueues(output, 6);
                                    break;
                                }
                                default: {
                                    device->addDisplayQueues("Recv Unknown", 6);
                                }
                            }
                            addPlotterQueue(packet); break;
                        }
                        case CMD_POST: {
                            device->addDisplayQueues("Recv CMD_POST: ", 6);
                            addPlotterQueue(packet); break;
                        }
                        case CMD_SYNC: {
                            device->addDisplayQueues("Recv CMD_SYNC: ", 6);
                            break;
                            // tweetSync.handleMessage(&content.syncItem); break;
                        }
                        case CMD_ATTENDANT: {
                            device->addDisplayQueues("Recv CMD_ATTENDANT: ", 6);
                            break;
                            // attendant.handleMessage(packet); break;
                        }
                    }
                };

                if (connected) {
                    udp.requestTime(&udpTimeRequestCb);
                }
            };

            servWifi.setupNetwork(_device);
            tweet.setup(&espNow);
            wServer.setup(&servWifi);
        }

        void addPlotterQueue(ReceivePacket2* packet) {
            queuePlotter.sendQueue(packet);
        }

        void handle_PlotterQueue() {
            ReceivePacket2 packet;
            Serial.println("\n----------------MsgQueue");

            if (queuePlotter.getQueue(&packet)) {
                switch (packet.dataPacket.msgType) {
                    case CMD_POST:
                        RecordItem record = packet.dataPacket.content.recordItem;
                        char output[256];
                        //! Make Json
                        record.makeJson(output, packet.sourceB);
                        Serial.println("Attempt Log Data");
                        Serial.println(output);
                        
                        Mng_Storage *storage = servWifi.interface->getStorage();
                        Data_Settings settings = storage->stoSettings.value;
                        Data_IotPlotter plotter = storage->stoPlotter.value;
                        
                        wServer.sendIotPlotter(plotter.apiKey, plotter.url, output);           
                        break;
                }
            }

        }

        void handleSingleClick() {
            tweet.command.sendSingleClick(22);
            // servWifi.tweet.sendSyncMock();
            // espNow.sendCustomPacket();
        }

        void handleDoubleClick() {
            tweet.command.sendDoubleClick(33);
            // servWifi.tweet.record.sendTempHumLux(2, 3, 4, 5, 6);
        }

        void handle_1secInterval() {
            // appTimer0.reset();
            // packetAnalyzer.draw(&(device->i2c1.disp));
            // AppPrint("1 anaylyzeTime", timer.elapsed());
        }

        void handle_PollNetworkState() {
            Network_State state = servWifi.pollNetworkState();
            if (state != NETWORK_FAILED && state != NETWORK_READY) return;
        }

        void handleRotary(RotaryDirection state, uint8_t counter) {
            if (state == CLOCKWISE) {
                // tweet.command.sendIr(222333);
                tweet.sendSyncMock(); 
            } else {
                // tweet.attendant.startAttendant();
                tweet.record.sendTempHumLux(2.3, 3.5, 4.5, 5.6, 6.7);
            }
        }

        void startAP(bool forceReset) {
            servWifi.wifi.startAP(forceReset, servWifi.scanChannel);
            espNow.changeChannel(servWifi.scanChannel);
        }

        void resetWifi() {
            servWifi.resetWifi();        //! Reset Wifi
        }

        void run() { 
            udp.run();
            espNow.run();
            wServer.run();
            // radio.run();
        }
};