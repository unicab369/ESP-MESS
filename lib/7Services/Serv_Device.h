// #define TTGO_LORA32 true
// #define DEV_KMC_70011 1
#include "Mng_Config.h"

class Interface_Device {
    public:
        virtual Mng_Storage* getStorage() { 
            Serial.println("ERR: NULL STORAGE");
            return NULL; 
        }

        virtual char* getDateStr() { 
            Serial.println("ERR: NULL DateStr");
            return "<Invalid>"; 
        }

        virtual void addDisplayQueues(const char*, uint8_t) {}
        virtual void handle_Interval(uint8_t, const char*, const char*)  {}

        virtual void updateTimer(time_t) {}
        virtual void handlePacket(ReceivePacket2*) {}
        virtual void handleAction(ControlOutput) {}
        virtual void toggleRelay() {}
};

class Serv_Device: public Serv_Serial, public Mng_Config, public Interface_Device {
    //! Pir Callback
    std::function<void(bool, uint8_t)> pirCb = [&](bool status, uint8_t pin) {
        Serial.print("PirTriggered = "); Serial.print(pin);
        Serial.print(" | "); Serial.println(status);
    };

    BNT_Hold releasedState = HOLD_TRANSITION;

    //! ButtonPress Callback
    std::function<void(BTN_Action, BNT_Hold, uint32_t)> buttonCb = 
                        [&](BTN_Action action, BNT_Hold hold, uint32_t elapse) {
        switch (action) {
            case ACTION_SINGLE_CLICK: {
                xSerial.println("Hellow there\n");
                addDisplayQueues("Single Click", 6);
                // network.handleSingleClick();
                onHandleSingleClick();
                toggleRelay();
                led.toggle();

                // servDev.buzzer.toggle();
                // led.toggle();

                // power.goToSleep(27, true);
                // if (power.wakeUpCauseResume()) {
                //     network.resetNetwork();
                // }

                break;
            }   
            case ACTION_DOUBLE_CLICK: {
                addDisplayQueues("Double Click", 6);
                // servDev.i2c1.switchDisplayMode();
                onHandleDoubleClick();
                break;
            }
            case ACTION_PRESS_ACTIVE: {
                //! Long press 20 sec -> Recovery/Factory Reset - turn off
                if (hold == HOLD_20_SEC) {
                    addDisplayQueues("[Bnt] 20sec hold", 6);
                    led.turnOFF();
                    storage.deleteAllData();

                //! Long press 10 sec -> Reset - fash flashing
                } else if (hold == HOLD_10_SEC) {
                    addDisplayQueues("[Bnt] 10sec hold", 6);
                    led.uniformPulse20ms();
                    releasedState = hold;

                //! Long press 5 sec -> Pairing - double flashing
                } else if (hold == HOLD_5_SEC) {
                    addDisplayQueues("[Bnt] 5sec hold", 6);
                    led.uniformPulse500ms();
                    releasedState = hold;
                }
                break;
            }
            case ACTION_PRESS_END: {
                char output[22];
                sprintf(output, "Press Ended %lu", elapse);
                addDisplayQueues(output, 6);

                if (releasedState == HOLD_5_SEC) {
                    onHandleStartAP();
                } else if (releasedState == HOLD_10_SEC) {
                    MY_ESP.restart();           //! Restart Device
                }
                break;
            }
        }
    };

    public:
        Serv_Device(): Serv_Serial() {}

        SerialControl serial;
            
        std::function<void()> onHandleSingleClick = [] {};
        std::function<void()> onHandleDoubleClick = [] {};
        std::function<void(RotaryDirection state, uint8_t counter)> onHandleRotary = [](RotaryDirection state, uint8_t counter) {};
        
        std::function<void()> onHandleResetWifi = [] {};
        std::function<void()> onHandleStartAP = [] {};

        //! Interfaces
        void addDisplayQueues(const char* str, uint8_t line) override {
            _addDisplayQueues(str, line);
        }

        void updateTimer(time_t time) override {
            appClock.updateTimers(time);
        }

        void handleAction(ControlOutput control) override {
            if (control.pin == 200) {
                led.handle(control);
            }
        }

        char* getDateStr() override {
            return appClock.getDateStr();
        }

        void toggleRelay() override {
            relay1.toggle();
        }

        void handle_Interval(uint8_t interval, const char* hostName, const char* localIP) override {
            // bool checkConn = i2c2.ch32v.checkConnection();
            // xLogf("I2C1 Connection = %d", checkConn);
            // i2c2.ch32v.requestReadings();
            // rtc2.run();
            // String timeStr = i2c1.rtc.timeDisplay();
            // // xLogf("TimeStr = %s", timeStr.c_str());
            // Serial.println(timeStr);
            // i2c1.as5600.run();
            // epaper.printLn();

            if (i2c1.dispMode == DISPLAY_DEFAULT) {
                _addDisplayQueues(appClock.getDisplay(), 1);         //* LINE 1  

                if (interval%2==0) {
                    i2c1.sensors.requestReadings();
                } 
                else if (interval%1==0) {
                    i2c1.sensors.collectReadings(); 
                    _addDisplayQueues(i2c1.sensors.getTempHumLux(), 5);  //* LINE 5
                    
                    if (interval%3==0) {
                        char strOut[22];
                        sprintf(strOut, "sd %luMB", storage.sd1.getCardSize());

                        _addDisplayQueues(hostName, 0);         //* LINE 0
                        _addDisplayQueues(strOut, 6);           //* LINE 6
                    } 
                    else {
                        char heapInfo[22];
                        char networkInfo[64];
                        sprintf(heapInfo, "a%lu/%lu/%lu", MY_ESP.maxAllocatedHeap(), ESP.getFreeHeap(), MY_ESP.heapSize());
                        uint64_t resetCount = storage.stoStat.resetCnt();
                        sprintf(networkInfo, "%s ~%u ~%llu", localIP, WiFi.channel(), resetCount);

                        _addDisplayQueues(networkInfo, 0);      //* LINE 0
                        _addDisplayQueues(heapInfo, 6);         //* LINE 6
                    }
                }

                // epaper.printLn();
            } 
            else if (i2c1.dispMode == DISPLAY_2ND) {

            }
        }

        //! configure
        void configure() {
            setup();
            setupSerial(this);

            edgeDetector.callback = &pirCb;
            button1.callback = &buttonCb;
        }

        //! Tweet CommandTrigger
        void handlePacket(ReceivePacket2* packet) {
            // storage.stoBehavior.handleCue(item->cue);
        }

        Mng_Storage* getStorage() {
            return &storage;
        }

        void runGroupTasks() {
            button1.run();

            //! Rotary Encoder
            rotary.run([&](RotaryDirection state, uint16_t counter) {
                char output[22];
                sprintf(output, "val = %lu dir %s", counter, (state == CLOCKWISE) ? "CW" : "CCW");
                AppPrint("\n[Rot]", output);
                addDisplayQueues(output, 6);       // display

                // i2c1.pca96.drivePWM(0, counter);
                onHandleRotary(state, counter);
            });

            //! Serial
            serial.run([&](char* inputStr) {
                if (strcmp(inputStr, "ping") == 0)  {
                    Serial.println("What is thy bidding my Master?");
                }       
                else if (strcmp("wifiReset", inputStr) == 0) {
                    onHandleResetWifi();
                }
                else if (strcmp("startAP", inputStr) == 0) {
                    onHandleStartAP();
                }
                else if (storage.handleConsoleCmd(inputStr) == RESET_WIFI) {
                } 
            });

            //! IRSwitch
            irSwitch.run([&](bool status, uint32_t value) {
                char output[22];
                sprintf(output, "Ir = %s", status ? String(value) : "Locked");
                AppPrint("\n[IR]", output);
                addDisplayQueues(output, 6);
            }); 

            // led.run();
            // ws2812.run();
            // xSerial.run();

            // buzzer.run();
            // edgeDetector.run();
        }
};