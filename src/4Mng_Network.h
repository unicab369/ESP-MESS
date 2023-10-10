#include "7WebServer/Web_Server.h"      // Flash +6%

class Mng_Network: public Serv_Network {
    Web_Server wServer;

    public:
        void handleSingleClick() {
            // char myStr[] = "Hello There!";
            // lora.sendData(myStr);
            tweet.command.sendSingleClick(22);
            tweet.sendSyncMock();
            // espNow.sendCustomPacket();
        }

        void handleDoubleClick() {
            tweet.command.sendDoubleClick(33);
            tweet.record.sendTempHumLux(2,3,4);
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
                tweet.command.sendIr(222333);   
            } else {
                tweet.attendant.startAttendant();
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
