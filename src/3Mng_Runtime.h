#include "4Mng_Network.h"

uint32_t loopCnt1 = 0, loopCnt2 = 0;
uint8_t secondCounter = 0;
uint32_t counter100ms = 0;

class Mng_Runtime: public Loggable {
    public:
        Mng_Runtime(): Loggable("Runtime") {}

        Serv_Device device;
        Mng_Network network;

        void setupRunTime() {
            xLogSection(__func__);

            device.configure();

            device.onHandleSingleClick = [&]() {
                network.handleSingleClick();
            };

            device.onHandleDoubleClick = [&]() {
                network.handleDoubleClick();
            };

            device.onHandleRotary = [&](RotaryDirection state, uint8_t counter) {
                network.handleRotary(state, counter);      // network message
            };

            device.onHandleResetWifi = [&]() {
                network.servWifi.resetWifi();
            };

            device.onHandleStartAP = [&]() {
                network.startAP(true);      //! Start access point
            };

            network.setup(&device);
        }

        unsigned long timeRef = millis();
        unsigned c0_timeRef = millis();
        unsigned c1_timeRef = millis();

        void run_core0() {
            loopCnt1++;
            network.run();

            if (millis()-c0_timeRef>1000) {
                char output[22];

                #if defined(ESP32)
                    uint32_t stackUsage = uxTaskGetStackHighWaterMark(NULL);
                    sprintf(output, "C0 D%lu +%lu", stackUsage, loopCnt1);
                #else
                    sprintf(output, "+%lu", loopCnt1);
                #endif

                device.addDisplayQueues(output, 3);
                loopCnt1 = 0;    
                c0_timeRef = millis();
            }
        }

        void run_core1() {
            loopCnt2++;
            device.runGroupTasks();

            //! guard
            if (millis() - timeRef < 100) return;
            timeRef = millis();
            device.handleQueues();

            #if defined(ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
                digitalWrite(22, !digitalRead(22));
            #else
                digitalWrite(2, !digitalRead(2));
            #endif

            counter100ms++;
            if (counter100ms>9) {
                counter100ms = 0;

                secondCounter++;
                if (secondCounter>59) secondCounter = 0;
                char output[22];

                #if defined(ESP32)
                    uint32_t stackUsage = uxTaskGetStackHighWaterMark(NULL);
                    sprintf(output, "C1 D%lu +%lu", stackUsage, loopCnt2);
                #endif

                device.addDisplayQueues(output, 4);

                //#cmd: iotPlotter
                //#cmd: apiKey
                //#cmd: url http://iotplotter.com/api/v2/feed/#
                //#cmd: settings
                uint8_t log_freq = device.getStorage()->stoSettings.value.espNowLogFreq;
                if (log_freq>2 && secondCounter%log_freq==0) {
                    network.handle_PlotterQueue();
                }

                uint8_t send_freq = device.getStorage()->stoSettings.value.espNowSendFreq;
                if (send_freq>2 && secondCounter%send_freq == 0) {
                    float temp, hum, lux, volt = 0, mA = 0;
                    device.i2c1.sensors.getTempHumLux(&temp, &hum, &lux);
                    // network->tweet.record.sendTempHumLux(temp, hum, lux, volt, mA);
                }

                if (device.i2c1.dispMode == DISPLAY_DEFAULT) {
                    device.addDisplayQueues(device.appClock.getDisplay(), 1);         //* LINE 1  

                    if (secondCounter%2==0) {
                        //! Request sensor Readings
                        device.i2c1.sensors.requestReadings();
                    } 
                    else if (secondCounter%1==0) {
                        //! Collect sensor Readings
                        device.i2c1.sensors.collectReadings(); 
                        device.addDisplayQueues(device.i2c1.sensors.getTempHumLux(), 5);  //* LINE 5
                        char strOut[22];

                        if (secondCounter%3==0) {
                            sprintf(strOut, "sd %luMB", device.storage.sd1.getCardSize());
                            device.addDisplayQueues(network.getHostName(), 0);         //* LINE 0
                        } else {
                            char networkInfo[64];
                            sprintf(strOut, "a%lu/%lu/%lu", MY_ESP.maxAllocatedHeap(), ESP.getFreeHeap(), MY_ESP.heapSize());
                            uint64_t resetCount = device.storage.stoStat.resetCnt();
                            sprintf(networkInfo, "%s ~%u ~%llu", network.getNetworkId(), WiFi.channel(), resetCount);

                            device.addDisplayQueues(networkInfo, 0);      //* LINE 0
                        }

                        device.addDisplayQueues(strOut, 6);           //* LINE 6
                    }

                    // epaper.printLn();
                } 
                else if (device.i2c1.dispMode == DISPLAY_2ND) {

                }

                network.handle_PollNetworkState();

                loopCnt2 = 0;
                c1_timeRef = millis();    
            }
        }
};