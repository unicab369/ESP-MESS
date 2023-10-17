enum Network_State {
    NETWORK_START,
    NETWORK_READY,
    NETWORK_FAILED,
    NETWORK_DONE,
    NETWORK_PROBING,
    NETWORK_UDP_DONE,
};

class Serv_Network: public Loggable {
    // ReceivePacketCb tweetHandler = [&](ReceivePacket* packet) {
    //     tweet.handleMessage(packet);
    // };

    std::function<void(ReceivePacket2*)> tweetHandler = [&](ReceivePacket2* packet) {
        tweet.handleMessage(packet);
    };

    std::function<void(time_t*)> udpTimeRequestCb = [&](time_t* time) {
        time_t _time = *time;
        device->appClock.updateTimers(_time);
        state = NETWORK_UDP_DONE;
        // udp.reload(&tweetHandler);
    }; 

    std::function<void(DataPacket2*)> onTweet2 = [&](DataPacket2* packet) {
        espNow.send(packet, sizeof(DataPacket2));
        // udp.broadcast(packet);
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
            tweet.updateChannel(WiFi.channel());

            //! load ESPNow callback
            espNow.callback2 = &tweetHandler;
            espNow.setup(WiFi.channel()); 

        } else if (retryCnt < 1) {
            output = "[Wifi] Err: Timeout";
            state = NETWORK_FAILED;

            //! load tweetSync callback
            tweet.tweetSync.onReceiveBounce = &onReceiveBounce;

            //! load ESPNow callback
            espNow.callback2 = &tweetHandler;
            espNow.setup(WiFi.channel()); 
        } 
        
        xLog(output.c_str());
        device->addDisplayQueues(output, 6);    //* LINE 6
    }

    void scanChannels() {
        xLog(__func__);

        scanChannel++;
        if (scanChannel>13) {
            state = NETWORK_DONE;
            return;
        }

        tweet.sendSyncMock();
        tweet.sendSyncMock();
        tweet.sendSyncMock();
    }

    Network_State state = NETWORK_START;
    uint8_t retryCnt = 0;

    public:
        Serv_Network(): Loggable("Net") {}

        Serv_Device *device;
        Net_Wifi wifi;
        Net_UDP udp;
        Net_Radio radio;
        Serv_EspNow espNow;
        Net_Lora lora;
        Serv_Tweet tweet;
        uint8_t scanChannel = 0;

        const char* getHostName() { return wifi.getHostName(); }

        void setupNetwork(Serv_Device *_device) {
            xLogSection(__func__);
            device = _device;
            tweet.setup(device, espNow.mac, &onTweet2);
            resetWifi();

            // radio.setup(5, 2);
            // const char *mqtt = "10.0.0.5";
            // mqtt_service.start(mqtt);
        }

        void resetWifi() {
            xLogLine(__func__);
            state = NETWORK_START;
            EEPROM_Extractor<WiFiCred> cred = device->storage.stoCred;
            retryCnt = 15;
            
            #ifdef ESP32
                if (digitalRead(36)) {
                    wifi.setup(cred.getValue()->ssid(), cred.getValue()->password());
                } else {
                    wifi.setup(cred.getValue()->ssid(), "cred.passw()");
                }
            #else
                wifi.setup(cred.ssid(), cred.passw());
            #endif
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
                    checkWifi(); 
                    break;
                } 
                default: break;
            }

            return state;
        }
};