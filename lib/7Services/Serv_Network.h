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

    std::function<void(ReceivePacket2*)> onEspNowCallback = [&](ReceivePacket2* packet) {
        tweet.handleMessage(packet);
    };

    std::function<void(time_t*)> udpTimeRequestCb = [&](time_t* time) {
        time_t _time = *time;
        interface->updateTimer(_time);
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

            // //! load ESPNow callback
            espNow.setup(WiFi.channel());
            if (*onWifiConnected) (*onWifiConnected)();

        } else if (retryCnt < 1) {
            output = "[Wifi] Err: Timeout";
            state = NETWORK_FAILED;

            //! load tweetSync callback
            tweet.tweetSync.onReceiveBounce = &onReceiveBounce;

            // //! load ESPNow callback
            espNow.setup(WiFi.channel()); 
        } 
        
        xLog(output.c_str());
        interface->addDisplayQueues(output, 6);    //* LINE 6
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

        const char* getHostName() { return wifi.getHostName(); }
        std::function<void()> *onWifiConnected;

        void setupNetwork(Interface_Device* _interface) {
            xLogSection(__func__);
            interface = _interface;
            tweet.setup(_interface, espNow.mac, &onTweet2);
            resetWifi();

            //! load ESPNow callback
            espNow.callback2 = &onEspNowCallback;

            // radio.setup(5, 2);
            // const char *mqtt = "10.0.0.5";
            // mqtt_service.start(mqtt);
        }

        void resetWifi() {
            xLogLine(__func__);
            state = NETWORK_START;
            retryCnt = 15;
            Data_Cred2 cred = interface->getStorage()->stoCred2.value;
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
                    checkWifi(); 
                    break;
                } 
                default: break;
            }

            return state;
        }
};