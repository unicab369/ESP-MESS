#include "7WebServer/Web_Server.h"      // Flash +6%

class Mng_Network: public Serv_Network {
    Web_Server wServer;

    void iotPlotter(uint32_t timeStamp, float temp, float hum, float lux, float volt, float mA) {
        wServer.makePostRequest(timeStamp, temp, hum, lux, volt, mA);
    }
    
    TweetRecordCb tweetRecordHandler = [&](float val1, float val2, float val3, float val4, float val5) {
        iotPlotter(0, val1, val2, val3, val4, val5);
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
        void setup(Serv_Device* device) {
            tweet.tweetRecordCb = &tweetRecordHandler;      //! ORDER DOES MATTER: need to assign callback bc it gets pass on
            setupNetwork(device);
        }

        void handleSingleClick() {
            // char myStr[] = "Hello There!";
            // lora.sendData(myStr);
            tweet.command.sendSingleClick(22);
            tweet.sendSyncMock();
            // espNow.sendCustomPacket();
        }

        void handleDoubleClick() {
            tweet.command.sendDoubleClick(33);
            tweet.record.sendTempHumLux(2, 3, 4, 5, 6);
        }

        void handle_1secInterval() {
            // appTimer0.reset();
            // packetAnalyzer.draw(&(device->i2c1.disp));
            // AppPrint("1 anaylyzeTime", timer.elapsed());
        }

        void handle_1secInterval_job2() {
            Network_State state = pollNetworkState();
            if (state != NETWORK_FAILED && state != NETWORK_READY) return;
            
            AppPrintSeparator("[Runtime]", "network configured");
            char *dateStr = device->appClock.getDateStr();
            AppPrint("\[Runtime]", "configure storage path = " + String(dateStr));
            device->storage.loadStoragePath(dateStr);
            AppPrintHeap();
            wServer.setup(this);
        }

        void handleRotary(RotaryDirection state, uint8_t counter) {
            if (state == CLOCKWISE) {
                // tweet.command.sendIr(222333);   
            } else {
                // tweet.attendant.startAttendant();
            }
        }

        void startAP(bool forceReset) {
            wifi.startAP(forceReset, scanChannel);
            espNow.changeChannel(scanChannel);
        }

        void run() { 
            udp.run();
            espNow.run();
            wServer.run();
            // radio.run();
        }
};
