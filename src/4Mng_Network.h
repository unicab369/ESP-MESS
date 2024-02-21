
#include "7WebServer/Web_Server.h"      // Flash +6%

class Mng_Network {
    Web_Server wServer;
    Net_Radio radio;
    Net_Lora lora;
    
    TweetRecordCb tweetRecordHandler = [&](float val1, float val2, float val3, float val4, float val5) {
        iotPlotter(val1, val2, val3, val4, val5);
    };

    public:
        Serv_Network servWifi;
        
        const char* getHostName()   { servWifi.getHostName(); }
        const char* getNetworkId()  { return servWifi.wifi.localIp().c_str(); }

        void setup(Serv_Device* device) {
            servWifi.tweet.tweetRecordCb = &tweetRecordHandler;      //! ORDER DOES MATTER

            servWifi.onWifiConnected = [&]() {
                char *dateStr = servWifi.interface->getDateStr();
                AppPrint("\n[Runtime]", "configure storage path = " + String(dateStr));
                servWifi.interface->getStorage()->loadStoragePath(dateStr);
                AppPrintHeap();
            };

            servWifi.setupNetwork(device);
            wServer.setup(&servWifi);
        }

        //! iotPlotter
        void iotPlotter(float temp, float hum, float lux, float volt, float mA) {
            Mng_Storage *storage = servWifi.interface->getStorage();
            Data_Settings settings = storage->stoSettings.value;
            Data_IotPlotter plotter = storage->stoPlotter.value;

            // Serial.print("SelfPlotEnable = "); Serial.println(settings.selfPlotEnable);

            if (settings.remotePlot) {
                wServer.makePostRequest(plotter.apiKey, plotter.feedId, temp, hum, lux, volt, mA);
            }
        }

        void handleSingleClick() {
            servWifi.tweet.command.sendSingleClick(22);
            servWifi.tweet.sendSyncMock();
            // espNow.sendCustomPacket();
        }

        void handleDoubleClick() {
            servWifi.tweet.command.sendDoubleClick(33);
            servWifi.tweet.record.sendTempHumLux(2, 3, 4, 5, 6);
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
            } else {
                // tweet.attendant.startAttendant();
            }
        }

        void startAP(bool forceReset) {
            servWifi.wifi.startAP(forceReset, servWifi.scanChannel);
            servWifi.espNow.changeChannel(servWifi.scanChannel);
        }

        void resetWifi() {
            servWifi.resetWifi();        //! Reset Wifi
        }

        void run() { 
            servWifi.udp.run();
            servWifi.espNow.run();
            wServer.run();
            // radio.run();
        }
};