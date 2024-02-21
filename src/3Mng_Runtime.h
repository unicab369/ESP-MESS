#include "4Mng_Network.h"

// #ifndef ESP32 
    #include <Ticker.h>
// #endif 

//! timerCallback is static sto asyncTimer1 cannot be an instance of Mng_Runtime
AsyncTimer asyncTimer1;

uint32_t loopCnt1 = 0, loopCnt2 = 0;

class Mng_Runtime: public Loggable {
    //! MAIN JOB
    std::function<void(RunTimeModel*)> runtimeCb1 = [&](RunTimeModel* runTime) {
        if (runTime->secondsChanged == true) {
            Serial.printf("\n\nTimer0 = %lu", loopCnt1);
            
            uint32_t stackUsage = uxTaskGetStackHighWaterMark(NULL);
            Serial.printf("\nstackSize = %lu", stackUsage);
            // String output = String(loopCnt1) + " " + String(stackUsage);
            // runtime->device.i2c1.disp.printline(output, 0);
            loopCnt1 = 0;
            
            // //! RUNTIME (inclusive): 1 second interval
            // device.render1s_Interval(&asyncTimer1, &asyncTimer2, [&]() {
                
            // });

            // if (runTime->isSecondInterval(5)) {
            //     char hostName[22];
            //     strcpy(hostName, network.getHostName());
            //     device.render5s_Interval(hostName);

            // } else if (runTime->isSecondInterval(3)) { 
            //     device.render3s_Interval(&asyncTimer1, &asyncTimer2);

            // } else if (runTime->isSecondInterval(2)) {
            //     device.render2s_Interval(network.getNetworkId());
            //     // network.handle_2secInterval();

            //     // //! Enable Rotary on 2nd second to prevent automatic trigger at the start
            //     device.rotary.isEnabled = true;
            // }
            
            // //! Reset sensors reading every second
            // device.addStoreQueue();

            // #ifndef ESP32 
            //     network.handle_PollNetworkState();
            // #endif
            
        } else {
            // //! RUNTIME: 25ms intervals
            // device.tick();

            // // #ifndef ESP32
            // //     device.runGroupTasks();
            // // #endif
        }
    };

    //! SECONDARY JOB
    std::function<void(RunTimeModel*)> runtimeCb2 = [&](RunTimeModel* runTime) {
        if (runTime->secondsChanged == true) {
            Serial.printf("\n\nTimer1*** = %lu", loopCnt2);

            uint32_t stackUsage = uxTaskGetStackHighWaterMark(NULL);
            Serial.printf("\nstackSize = %lu", stackUsage);
            // String output = String(loopCnt2) + " " + String(stackUsage);
            // runtime->device.i2c1.disp.printline(output, 1);

            loopCnt2 = 0;

            // //! RUNTIME: 1 second intervals
            // network.handle_PollNetworkState();

            // float temp, hum, lux;
            // device.i2c1.sensors.getTempHumLux(&temp, &hum, &lux);
            // // wServer.refreshReadings(temp, hum, lux);

            // if (runTime->isSecondInterval(5)) {
            //     DataContent content;

            //     // while (device.queuePlotter.getQueue(&content)) {
            //     //     RecordItem item = content.recordItem;

            //     //     switch (item.cue) {
            //     //         case RECORD_TEMPHUMLUX: {
            //     //             AppPrint("\nRECORD_TEMPHUMLUX 22", "T=" + String(item.value1) + 
            //     //                 ", H=" + String(item.value2) + ", L=" + String(item.value3) +
            //     //                 ", V=" + String(item.value4) + ", mA=" + String(item.value5));
            //     //             //     if (callback != nullptr) (*callback)(item->value1, item->value2, item->value3, item->value4, item->value5);
            //     //             break;
            //     //         }
            //     //         default: {
            //     //             AppPrint("Receiv Unkwnown");
            //     //             break;
            //     //         }
            //     //     }
            //     // }
            //     // device.
            // } 
            // // else if (runTime->isSecondInterval(5)) {
            // //     // network.iotPlotter(temp, hum, lux, 0, 0);
            // // }

        } else {
            //! RUNTIME: 25ms intervals
            // device.runGroupTasks();
        }
    };

    #if defined(ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
        static void onRefreshInterval(void* parameter) {
            RunTimeModel *param = (RunTimeModel*)parameter;

            for (;;) {
                vTaskDelay(pdMS_TO_TICKS(25));
                param->millisec += 25;
            }
        }
    #else
        Ticker timer;

        static void timerCallback() {
            asyncTimer1.model.millisec += 25;
        }
    #endif

    public:
        Mng_Runtime(): Loggable("Runtime") {}

        AsyncTimer asyncTimer2;
        Serv_Device device;
        Mng_Network network;
        Mng_Power power;

        void setupRunTime() {
            xLogSection(__func__);

            device.configure();

            device.onHandleSingleClick = [&]() {
                Serial.println("SINGLE CLICK A");
                // network.handleSingleClick();
            };

            device.onHandleDoubleClick = [&]() {
                Serial.println("DOUBLE CLICK B");
                // network.handleDoubleClick();
            };

            device.onHandleRotary = [&](RotaryDirection state, uint8_t counter) {
                // network.handleRotary(state, counter);      // network message
            };

            device.onHandleResetWifi = [&]() {
                network.resetWifi();
            };

            device.onHandleStartAP = [&]() {
                network.startAP(true);      //! Start access point
            };

            pinMode(22, OUTPUT);

            network.setup(&device);

            #if defined(ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
                //! create task that run every 25ms on both cores
                // xTaskCreatePinnedToCore(Mng_Runtime::onRefreshInterval, "myTask1", 1000, &(asyncTimer1.model), 1, NULL, 0);
                // xTaskCreatePinnedToCore(Mng_Runtime::onRefreshInterval, "myTask2", 1000, &(asyncTimer2.model), 1, NULL, 1);

                xTaskCreatePinnedToCore([](void *pvParam){
                    Mng_Network* network = (Mng_Network*)pvParam;

                    while(1) {
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        uint32_t stackUsage = uxTaskGetStackHighWaterMark(NULL);
                        String output = "C0: " + String(loopCnt1) + " " + String(stackUsage);
                        network->servWifi.interface->addDisplayQueues(output, 3);
                        
                        // Serial.printf("\n\nTimer0 = %lu; stack = %lu", loopCnt1, stackUsage);
                        loopCnt1 = 0;

                        
                    }
                }, "core0_Task2", 15000, &network, 1, NULL, 0);

                xTaskCreatePinnedToCore([](void *pvParam){
                    Mng_Network* network = (Mng_Network*)pvParam;

                    while(1) {
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        uint32_t stackUsage = uxTaskGetStackHighWaterMark(NULL);
                        String output = "C1: " + String(loopCnt2) + " " + String(stackUsage);
                        network->servWifi.interface->addDisplayQueues(output, 4);

                        // Serial.printf("\n\nTimer1*** = %lu; stack = %lu", loopCnt2, stackUsage);
                        loopCnt2 = 0;

                        network->handle_PollNetworkState();
                    }
                }, "core1_Task2", 7000, &network, 1, NULL, 1);
            #else
                timer.attach(0.025, Mng_Runtime::timerCallback);
            #endif
        }

        unsigned long timeRef = millis();

        void runJob1() {
            loopCnt1++;
            network.run();

            // //! JOB1
            // asyncTimer1.run(runtimeCb1);
        }

        void runJob2() {
            loopCnt2++;

            if (millis() - timeRef > 100) {
                digitalWrite(22, !digitalRead(22));
                timeRef = millis();
                device.handleQueues();
            }

            device.runGroupTasks();

            //! JOB2: LIVE TASKS
            // asyncTimer2.run(runtimeCb2);
        }
};

// Lora Bluetooth, mqtt
// FS update, sd storage
// device encryption

// ESP12
// Servo                (2)

// device discovery
// multi channel
// - Storing channel
// - Scan channel
// - broadcast channel

// PIR Multi            (4)
// handler:
// x toggle pin
// ws2812 output
// buzzer

//! ESPNow, behavior     (3)
//! Manage pins