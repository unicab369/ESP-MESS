enum Network_State {
    NETWORK_START,
    NETWORK_READY,
    NETWORK_FAILED,
    NETWORK_DONE,
    NETWORK_PROBING,
};

class Serv_Network: public Loggable {
    // std::function<void(uint8_t)> onReceiveBounce = [&](uint8_t destChannel) {
    //     if (scanChannel != destChannel) return;
    //     AppPrint("\n\n\n**** FOUND ZZZZZ 111");
    //     state = NETWORK_DONE;
    //     tweet.tweetSync.onReceiveBounce = NULL;
    // };

    void checkWifi() {
        if (retryCnt < 0) return;
        String output = "[Wifi] Conn ... " + String(retryCnt);
        retryCnt--;
        
        if (wifi.isConnected()) {
            output = "[Wifi] CONNECTED";
            state = NETWORK_READY;
            AppPrint("IP Addr", wifi.localIp());

            // tweet.updateChannel(WiFi.channel());
            onWifiConnected(true);

        } else if (retryCnt < 1) {
            output = "[Wifi] Err: Timeout";
            state = NETWORK_FAILED;

            // tweet.tweetSync.onReceiveBounce = &onReceiveBounce;
            onWifiConnected(false);
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
        uint8_t scanChannel = 0;

        std::function<void(boolean)> onWifiConnected = [](boolean){};

        void setupNetwork(Interface_Device* _devInterface) {
            xLogSection(__func__);

            interface = _devInterface;
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