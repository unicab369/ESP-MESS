#include "4Mng_Network.h"

// #ifndef ESP32 
    #include <Ticker.h>
// #endif 

//! timerCallback is static sto asyncTimer1 cannot be an instance of Mng_Runtime
AsyncTimer asyncTimer1;

uint32_t loopCnt1 = 0, loopCnt2 = 0;
uint8_t secondCounter = 0;

class Mng_Runtime: public Loggable {
    //! MAIN JOB
    std::function<void(RunTimeModel*)> runtimeCb1 = [&](RunTimeModel* runTime) {
        if (runTime->secondsChanged == true) {            
            // //! Reset sensors reading every second
            // device.addStoreQueue();
        }
    };

    //! SECONDARY JOB
    std::function<void(RunTimeModel*)> runtimeCb2 = [&](RunTimeModel* runTime) {
        if (runTime->secondsChanged == true) {
            // if (runTime->isSecondInterval(5)) {
            //     DataContent content;

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
                network.handleSingleClick();
            };

            device.onHandleDoubleClick = [&]() {
                network.handleDoubleClick();
            };

            device.onHandleRotary = [&](RotaryDirection state, uint8_t counter) {
                network.handleRotary(state, counter);      // network message
            };

            device.onHandleResetWifi = [&]() {
                network.resetWifi();
            };

            device.onHandleStartAP = [&]() {
                network.startAP(true);      //! Start access point
            };

            network.setup(&device);

            #if defined(ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
                pinMode(22, OUTPUT);

                //! create task that run every 25ms on both cores
                // xTaskCreatePinnedToCore(Mng_Runtime::onRefreshInterval, "myTask1", 1000, &(asyncTimer1.model), 1, NULL, 0);
                // xTaskCreatePinnedToCore(Mng_Runtime::onRefreshInterval, "myTask2", 1000, &(asyncTimer2.model), 1, NULL, 1);

                xTaskCreatePinnedToCore([](void *pvParam){
                    Mng_Network* network = (Mng_Network*)pvParam;
                    Serv_Device *dev = network->device;

                    while(1) {
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        uint32_t stackUsage = uxTaskGetStackHighWaterMark(NULL);

                        char output[22];
                        sprintf(output, "C0 D%lu +%lu", stackUsage, loopCnt1);
                        dev->addDisplayQueues(output, 3);
                        // Serial.printf("\n\nTimer0 = %lu; stack = %lu", loopCnt1, stackUsage);
                        loopCnt1 = 0;
                    }
                }, "core0_TaskB", 4000, &network, 1, NULL, 0);

                xTaskCreatePinnedToCore([](void *pvParam){
                    Mng_Network* network = (Mng_Network*)pvParam;
                    Serv_Device *dev = network->device;

                    while(1) {
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        uint32_t stackUsage = uxTaskGetStackHighWaterMark(NULL);

                        char output[22];
                        sprintf(output, "C1 D%lu +%lu", stackUsage, loopCnt2);
                        dev->addDisplayQueues(output, 4);
                        // Serial.printf("\n\nTimer1*** = %lu; stack = %lu", loopCnt2, stackUsage);
                        loopCnt2 = 0;
                        
                        secondCounter++;
                        if (secondCounter>59) secondCounter = 0;

                        //#cmd: settings
                        uint8_t log_freq = dev->getStorage()->stoSettings.value.espNowLogFreq;

                        if (log_freq>2 && secondCounter%log_freq==0) {
                            network->handle_PlotterQueue();
                        }

                        //#cmd: iotPlotter
                        //#cmd: apiKey
                        //#cmd: url http://iotplotter.com/api/v2/feed/
                        uint8_t send_freq = dev->getStorage()->stoSettings.value.espNowSendFreq;

                        if (send_freq>2 && secondCounter%send_freq == 0) {
                            float temp, hum, lux, volt = 0, mA = 0;
                            dev->i2c1.sensors.getTempHumLux(&temp, &hum, &lux);
                            network->tweet.record.sendTempHumLux(temp, hum, lux, volt, mA);
                        }

                        dev->handle_Interval(secondCounter, network->getHostName(), network->getNetworkId());
                        network->handle_PollNetworkState();
                    }
                }, "core1_TaskB", 4000, &network, 1, NULL, 1);
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
                #if defined(ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
                    digitalWrite(22, !digitalRead(22));
                #endif
                
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