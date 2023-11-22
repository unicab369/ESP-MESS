#include "7WebServer/Web_Server.h"      // Flash +6%

class Mng_Network {
    Web_Server wServer;
    Net_Radio radio;
    Net_Lora lora;

    Serv_Device *device() { return servWifi.device; }
    
    TweetRecordCb tweetRecordHandler = [&](float val1, float val2, float val3, float val4, float val5) {
        iotPlotter(val1, val2, val3, val4, val5);
    };

    std::function<void(ReceivePacket2*)> onHandleTweet = [&](ReceivePacket2* packet) {
        DataContent& content = packet->dataPacket.content;

        switch (packet->dataPacket.info.sourceCmd) {
            case CMD_POST: {
                break;
            }
            default: break;
        }
    };

    public:
        Serv_Network servWifi;
        
        void setup(Serv_Device* device) {
            servWifi.tweet.tweetRecordCb = &tweetRecordHandler;      //! ORDER DOES MATTER: need to assign callback bc it gets pass on
            servWifi.setupNetwork(device);
        }

        //! iotPlotter
        void iotPlotter(float temp, float hum, float lux, float volt, float mA) {
            Mng_Storage storage = servWifi.device->storage;
            Dat_Settings settings = storage.stoSettings.value;
            Dat_Plotter plotter = storage.stoPlotter.value;

            // Serial.print("SelfPlotEnable = "); Serial.println(settings.selfPlotEnable);

            if (settings.selfPlotEnable) {
                wServer.makePostRequest(plotter.apiKey, plotter.feedId, temp, hum, lux, volt, mA);
            }
        }

        void handleSingleClick() {
            // char myStr[] = "Hello There!";
            // lora.sendData(myStr);
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

        void handle_1secInterval_job2() {
            Network_State state = servWifi.pollNetworkState();
            if (state != NETWORK_FAILED && state != NETWORK_READY) return;
            
            AppPrintSeparator("[Runtime]", "network configured");
            // char *dateStr = servWifi.device->appClock.getDateStr();
            // AppPrint("\[Runtime]", "configure storage path = " + String(dateStr));
            // servWifi.device->storage.loadStoragePath(dateStr);
            AppPrintHeap();
            wServer.setup(&servWifi);
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

        void run() { 
            servWifi.udp.run();
            servWifi.espNow.run();
            wServer.run();
            // radio.run();
        }
};
