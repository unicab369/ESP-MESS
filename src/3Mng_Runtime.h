#include "4Mng_Network.h"

// #ifndef ESP32 
    #include <Ticker.h>
// #endif 

//! timerCallback is static sto asyncTimer1 cannot be an instance of Mng_Runtime
AsyncTimer asyncTimer1;

class Mng_Runtime: public Loggable {
    //! MAIN JOB
    std::function<void(RunTimeModel*)> runtimeCb1 = [&](RunTimeModel* runTime) {
        if (runTime->secondsChanged == true) {
            #if defined(ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
                // //! Code crashes without feeding watchdog. Why?
                // TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
                // TIMERG0.wdt_feed=1;
                // TIMERG0.wdt_wprotect=0;
            #endif

            //! RUNTIME (inclusive): 1 second interval
            device.render1s_Interval(&asyncTimer1, &asyncTimer2, [&]() {
                
            });

            if (runTime->isSecondInterval(5)) {
                char hostName[22];
                strcpy(hostName, network.getHostName());
                device.render5s_Interval(hostName);

            } else if (runTime->isSecondInterval(3)) { 
                device.render3s_Interval(&asyncTimer1, &asyncTimer2);

            } else if (runTime->isSecondInterval(2)) {
                device.render2s_Interval(network.getNetworkId());
                // network.handle_2secInterval();

                // //! Enable Rotary on 2nd second to prevent automatic trigger at the start
                device.rotary.isEnabled = true;
            }
            
            //! Reset sensors reading every second
            device.addStoreQueue();

            #ifndef ESP32 
                network.handle_1secInterval_job2();
            #endif
            
        } else {
            //! RUNTIME: 25ms intervals
            device.tick();

            #ifndef ESP32
                device.runGroupTasks();
            #endif
        }
    };

    //! SECONDARY JOB
    std::function<void(RunTimeModel*)> runtimeCb2 = [&](RunTimeModel* runTime) {
        if (runTime->secondsChanged == true) {
            //! RUNTIME: 1 second intervals
            network.handle_1secInterval_job2();

            float temp, hum, lux;
            device.i2c1.sensors.getTempHumLux(&temp, &hum, &lux);
            // wServer.refreshReadings(temp, hum, lux);

            if (runTime->isSecondInterval(5)) {
                network.iotPlotter(temp, hum, lux, 0, 0);
            }

        } else {
            //! RUNTIME: 25ms intervals
            device.runGroupTasks();
        }
    };

    std::function<void()> onHandleSingleClick = [&]() {
        network.handleSingleClick();
    };

    std::function<void()> onHandleDoubleClick = [&]() {
        network.handleDoubleClick();
    };

    std::function<void()> onHandleAPRequest = [&]() {
        network.startAP(true);      //! Start access point
    };

    std::function<void()> onHandleResetWifi = [&]() {
        network.resetWifi();
    };

    std::function<void(RotaryDirection state, uint8_t counter)> onHandleRotary = 
            [&](RotaryDirection state, uint8_t counter) {
        network.handleRotary(state, counter);      // network message
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
            device.onHandleSingleClick = &onHandleSingleClick;
            device.onHandleDoubleClick = &onHandleDoubleClick;
            device.onHandleAPRequest = &onHandleAPRequest;
            device.onHandleResetWifi = &onHandleResetWifi;
            device.onHandleRotary = &onHandleRotary;

            network.setup(&device);

            //! setup AsyncTimers
            asyncTimer1.setup(&runtimeCb1);
            asyncTimer2.setup(&runtimeCb2);

            #if defined(ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
                //! create task that run every 25ms on both cores
                xTaskCreatePinnedToCore(Mng_Runtime::onRefreshInterval, "myTask1", 1000, &(asyncTimer1.model), 1, NULL, 0);
                xTaskCreatePinnedToCore(Mng_Runtime::onRefreshInterval, "myTask2", 1000, &(asyncTimer2.model), 1, NULL, 1);
            #else
                timer.attach(0.025, Mng_Runtime::timerCallback);
            #endif
        }

        void runJob1() {
            //! JOB1
            asyncTimer1.run();
        }

        void runJob2() {
            //! JOB2: LIVE TASKS
            network.run();
            device.runMainTask2();

            #if defined(ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
                asyncTimer2.run();
            #endif
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