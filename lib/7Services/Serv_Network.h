enum Network_State {
    NETWORK_START,
    NETWORK_READY,
    NETWORK_FAILED,
    NETWORK_DONE,
    NETWORK_PROBING,
    NETWORK_UDP_DONE,
};

class Serv_Network: public Loggable {
    std::function<void(time_t*)> udpTimeRequestCb = [&](time_t* time) {
        time_t _time = *time;
        interface->updateTimer(_time);
        state = NETWORK_UDP_DONE;
    }; 

    std::function<void(uint8_t)> onReceiveBounce = [&](uint8_t destChannel) {
        if (scanChannel != destChannel) return;
        AppPrint("\n\n\n**** FOUND ZZZZZ 111");
        state = NETWORK_DONE;
        tweet.tweetSync.onReceiveBounce = NULL;
    };

    void checkWifi() {
        if (retryCnt < 0) return;
        String output = "[Wifi] Conn ... " + String(retryCnt);
        retryCnt--;
        
        if (wifi.isConnected()) {
            output = "[Wifi] CONNECTED";
            state = NETWORK_READY;
            AppPrint("IP Addr", wifi.localIp());
            
            udp.requestTime(&udpTimeRequestCb);
            // tweet.updateChannel(WiFi.channel());

            //! load ESPNow callback
            espNow.setup(WiFi.channel());
            onWifiConnected();

        } else if (retryCnt < 1) {
            output = "[Wifi] Err: Timeout";
            state = NETWORK_FAILED;

            //! load tweetSync callback
            // tweet.tweetSync.onReceiveBounce = &onReceiveBounce;

            // //! load ESPNow callback
            espNow.setup(WiFi.channel()); 
        } 
        
        xLog(output.c_str());
        interface->addDisplayQueues(output.c_str(), 6);    //* LINE 6
    }

    void scanChannels() {
        xLog(__func__);

        scanChannel++;
        if (scanChannel>13) {
            state = NETWORK_DONE;
            return;
        }
    }

    Network_State state = NETWORK_START;
    uint8_t retryCnt = 0;

    public:
        Serv_Network(): Loggable("Net") {}

        Interface_Device *interface;
        Net_Wifi wifi;
        Net_UDP udp;
        Serv_EspNow espNow;
        Serv_Tweet tweet;
        uint8_t scanChannel = 0;

        std::function<void()> onWifiConnected = [](){};

        void setupNetwork(Interface_Device* _devInterface) {
            xLogSection(__func__);
            interface = _devInterface;
            tweet.setup(&espNow);

            espNow.callback = [&](ReceivePacket2* packet) {
                DataContent content = packet->dataPacket.content;

                switch (packet->dataPacket.info.sourceCmd) {
                    case CMD_TRIGGER: {
                        CommandItem item = packet->dataPacket.content.commandItem;

                        switch (item.cue) {
                            case TRIGGER_STARTUP: {
                                interface->addDisplayQueues("Recv Startup", 6);
                                break;
                            }
                            case TRIGGER_SINGLECLICK: {
                                char output[22];
                                sprintf(output, "Recv Single: %lu", item.value);
                                interface->addDisplayQueues(output, 6);
                                // device->led.toggle();
                                break;
                            }
                            case TRIGGER_DOUBLECLICK: {
                                char output[22];
                                sprintf(output, "Recv Double: %lu", item.value);
                                interface->addDisplayQueues(output, 6);
                                // device->led.repeatPulses(1000);
                                break;  
                            }
                            case TRIGGER_PIR: {
                                char output[22];
                                sprintf(output, "Recv Pir: %lu", item.value);
                                interface->addDisplayQueues(output, 6);
                                break;
                            }
                            case TRIGGER_IR: {
                                char output[22];
                                sprintf(output, "Recv Ir: %lu", item.value);
                                interface->addDisplayQueues(output, 6);
                                break;
                            }
                            default: {
                                interface->addDisplayQueues("Recv Unknown", 6);
                            }
                        }
                        interface->addPlotterQueue(packet); break;
                    }
                    case CMD_POST: {
                        interface->addDisplayQueues("Recv CMD_POST: ", 6);
                        interface->addPlotterQueue(packet); break;
                    }
                    case CMD_SYNC: {
                        interface->addDisplayQueues("Recv CMD_SYNC: ", 6);
                        break;
                        // tweetSync.handleMessage(&content.syncItem); break;
                    }
                    case CMD_ATTENDANT: {
                        interface->addDisplayQueues("Recv CMD_ATTENDANT: ", 6);
                        break;
                        // attendant.handleMessage(packet); break;
                    }
                }
            };

            resetWifi();

            // radio.setup(5, 2);
            // const char *mqtt = "10.0.0.5";
            // mqtt_service.start(mqtt);
        }

        void resetWifi() {
            xLogLine(__func__);
            state = NETWORK_START;
            retryCnt = 15;
            Data_Cred cred = interface->getStorage()->stoCred.value;
            wifi.setup(cred.ssid, cred.password);
        }

        Network_State pollNetworkState() {
            switch (state) {
                case NETWORK_PROBING: {
                    // scanChannels();
                    break;
                }
                case NETWORK_FAILED: {
                    state = NETWORK_PROBING;
                    scanChannel = 1;
                    break;
                }
                
                case NETWORK_READY: {
                    state = NETWORK_DONE;
                    break;
                }

                case NETWORK_START: {
                    checkWifi();        // -> NETWORK_READY or NETWORK_FAILED
                    break;
                } 
                default: break;
            }

            return state;
        }
};