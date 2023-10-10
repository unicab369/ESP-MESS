#ifndef ESP32 
    #include <Ticker.h>
#endif 

//! timerCallback is static sto asyncTimer1 cannot be an instance of Mng_Runtime
AsyncTimer asyncTimer1;

class Mng_Runtime: public BaseComponent {
    //! 1 Second Interval
    void render1s_Interval() {
        if (device->i2c1.dispMode == DISPLAY_DEFAULT) {
            device->addDisplayQueue1(device->appClock.getDisplay(), 1);      //* LINE 1   
            device->addDisplayQueue1(asyncTimer1.record(), 2);                 //* LINE 2
            device->addDisplayQueue1(asyncTimer2.record(), 3);                 //* LINE 3
            device->addDisplayQueue1(device->i2c1.sensors.getTempHumLux(), 5); //* LINE 5

        } else if (device->i2c1.dispMode == DISPLAY_2ND) {
            network->handle_1secInterval();
        }

        device->addDisplayQueue2(device->appClock.getDisplay(), 1);      //* LINE 1
        device->addDisplayQueue2(asyncTimer1.record(), 2);                 //* LINE 2
        device->addDisplayQueue2(asyncTimer2.record(), 3);                 //* LINE 3
        device->addDisplayQueue2(device->i2c1.sensors.getTempHumLux(), 5); //* LINE 5
    }

    //! 5 Seconds Interval
    void render5s_Interval() {
        char hostName[22], sdSize[22];
        sprintf(sdSize, "sd = %u MB", device->storage.sd1.getCardSize());
        strcpy(hostName, network->getHostName());

        if (device->i2c1.dispMode == DISPLAY_DEFAULT) {
            device->addDisplayQueue1(hostName, 0);     //! Oled Mini LINE 0  
            device->addDisplayQueue1(sdSize, 4);       //! Oled Mini LINE 4  
            device->addDisplayQueue1("Heap: " + String(ESP.getFreeHeap()), 6);     //* LINE 6
        }
        device->addDisplayQueue2(hostName, 0);         //* LINE 0
        device->addDisplayQueue2(sdSize, 4);           //* LINE 4
        device->addDisplayQueue2("Heap: " + String(ESP.getFreeHeap()), 6);     //* LINE 6
    }

    //! 3 Seconds Interval
    void render3s_Interval() {
        char output[22];
        sprintf(output, "dep = %u/%u", asyncTimer1.stackUsage, asyncTimer2.stackUsage);

        if (device->i2c1.dispMode == DISPLAY_DEFAULT) {
            // showLadderId(); // line0
            device->addDisplayQueue1(String(output), 4);   //! Oled Mini LINE 4
        }
        device->addDisplayQueue2(output, 4);               //* LINE 4
    }

    //! 2 Seconds Interval
    void render2s_Interval() {
        char heapInfo[22], networkInfo[32];
        sprintf(heapInfo, "mem = %u/%u", MY_ESP.maxHeap(), ESP.getFreeHeap());

        uint64_t cnt = device->storage.resetCount.value;
        xLog("IM HERE zzz ", cnt);

        // sprintf(networkInfo, "%s ~%u ~%llu", network->wifi.localIp(), 
        //             WiFi.channel(), device->storage.resetCount.getValue());
        sprintf(networkInfo, "%s ~%u ~%llu", network->wifi.localIp(), 
                            WiFi.channel(), 33);

        //! Oled Mini
        if (device->i2c1.dispMode == DISPLAY_DEFAULT) {
            device->addDisplayQueue1(networkInfo, 0);      //! Oled Mini LINE 0
            device->addDisplayQueue1(heapInfo, 4);   //! Oled Mini LINE 4        
        }
        device->addDisplayQueue2(networkInfo, 0);          //* LINE 0
        device->addDisplayQueue2(heapInfo, 4);               //* LINE 4
    }

    //! MAIN JOB
    std::function<void(RunTimeModel*)> runtimeCb1 = [&](RunTimeModel* runTime) {
        if (runTime->secondsChanged == true) {
            //! RUNTIME (inclusive): 1 second interval
            render1s_Interval();

            if (runTime->isSecondInterval(5)) {
                render5s_Interval();                    //! RUNTIME: 5 seconds intervals
            } else if (runTime->isSecondInterval(3)) {
                render3s_Interval();                    //! RUNTIME: 3 seconds intervals 
            } else if (runTime->isSecondInterval(2)) {
                render2s_Interval();                    //! RUNTIME: 2 seconds intervals

                //! Enable Rotary on 2nd second to prevent automatic trigger at the start
                rotary.isEnabled = true;    
            } else {
                //! RUNTIME (exclusive): 1 second interval
            }

            //! Reset sensors reading every second
            device->addStoreQueue();
            device->i2c1.sensors.reset();

            #ifndef ESP32 
                network->handle_1secInterval_job2();
            #endif
            
        } else {
            //! RUNTIME: 25ms intervals
            device->tick();

            #ifndef ESP32
                device->runGroupTasks();
            #endif
        }
    };

    //! SECONDARY JOB
    std::function<void(RunTimeModel*)> runtimeCb2 = [&](RunTimeModel* runTime) {
        if (runTime->secondsChanged == true) {
            //! RUNTIME: 1 second intervals
            network->handle_1secInterval_job2();

            float temp, hum, lux;
            device->i2c1.sensors.getTempHumLux(&temp, &hum, &lux);
            // wServer.refreshReadings(temp, hum, lux);
        } else {
            //! RUNTIME: 25ms intervals
            device->runGroupTasks();
        }
    };

    RotaryEncoder rotary;
    
    std::function<void(RotaryDirection, uint8_t)> rotaryCb = [&](RotaryDirection state, uint8_t counter) {
        #ifdef ESP32
            String readings = "IO36=" + String(digitalRead(36)) + " IO39=" + String(digitalRead(39));
            AppPrint("Read ", readings);
            device->addDisplayQueues(readings, 5);
        #endif

        String dir = (state == CLOCKWISE) ? "CW" : "CCW";
        String output =  "val=" + String(counter) + " Dir=" + dir;
        AppPrint("[Rot]", output);
        device->addDisplayQueues(output, 6);       // display
        network->handleRotary(state, counter);      // network message
    };

    #ifdef ESP32
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
        Mng_Runtime(): BaseComponent("Runtime") {}

        AsyncTimer asyncTimer2;
        Serv_Device *device;
        Mng_Network *network;

        void setup(Serv_Device *_servDev, Mng_Network *_network) {
            xLog(__func__);
            device = _servDev;
            network = _network;
            rotary.setup(device->P_CONF.rotaryA, device->P_CONF.rotaryB);
            rotary.onCallback = &rotaryCb;

            //! setup AsyncTimers
            asyncTimer1.setup(&runtimeCb1);
            asyncTimer2.setup(&runtimeCb2);

            #ifdef ESP32
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
            // //! JOB2: LIVE TASKS
            // network->run();
            // rotary.run();

            // #ifdef ESP32
            //     asyncTimer2.run();
            // #endif
        }
};
