#include "4Mng_Network.h"
#include "3Mng_Runtime.h"

class Mng_Device: public BaseComponent {
    std::function<void(BTN_Action, BNT_Hold, uint32_t)> buttonCb = 
                        [&](BTN_Action action, BNT_Hold hold, uint32_t elapse) {
        switch (action) {
            case ACTION_SINGLE_CLICK: {
                servDev.xSerial.println("Hellow there\n");
                servDev.addDisplayQueues("Single Click", 6);
                network.handleSingleClick();
                servDev.toggleRelay();

                // uint8_t read = digitalRead(14);
                // digitalWrite(14, !read);

                // servDev.buzzer.toggle();
                // servDev.storage.testSetupData();
                // led.toggle();

                // power.goToSleep(27, true);

                // if (power.wakeUpCauseResume()) {
                //     network.resetNetwork();
                // }

                break;
            }   
            case ACTION_DOUBLE_CLICK: {
                servDev.addDisplayQueues("Double Click", 6);
                // servDev.i2c1.switchDisplayMode();
                network.handleDoubleClick();
                break;
            }
            case ACTION_PRESS_ACTIVE: {
                //! Long press 20 sec -> Recovery/Factory Reset - turn off
                if (hold == HOLD_20_SEC) {
                    servDev.addDisplayQueues("[Bnt] 20sec hold", 6);
                    servDev.led.setOFF();
                    servDev.storage.deleteData();

                //! Long press 10 sec -> Reset - fash flashing
                } else if (hold == HOLD_10_SEC) {
                    servDev.addDisplayQueues("[Bnt] 10sec hold", 6);
                    servDev.led.repeatPulses(400);
                    releasedState = hold;

                //! Long press 5 sec -> Pairing - double flashing
                } else if (hold == HOLD_5_SEC) {
                    servDev.addDisplayQueues("[Bnt] 5sec hold", 6);
                    servDev.led.repeatPulses(20);
                    releasedState = hold;
                }
                break;
            }
            case ACTION_PRESS_END: {
                servDev.addDisplayQueues("Press Ended " + String(elapse), 6);
                if (releasedState == HOLD_5_SEC) {
                    MY_ESP.restart();       //! Restart Device
                } else if (releasedState == HOLD_10_SEC) {
                    network.startAP(true);      //! Start access point
                }
                break;
            }
        }
    };

    std::function<void(bool, uint32_t)> irSwitchCb = [&](bool status, uint32_t value) {
        String output = "IrRead = " + (status ? String(value) : "Locked");
        AppPrint("[IR]", output);
        servDev.addDisplayQueue1(output, 6);
    };

    std::function<void(bool, uint8_t)> pirCb = [&](bool status, uint8_t pin) {
        Serial.print("PirTriggered = "); Serial.print(pin);
        Serial.print(" | "); Serial.println(status);
    };

    std::function<void(const char* ssid, const char *passw)> 
            storeCredCb = [&](const char* ssid, const char *passw) {
        servDev.storage.stoCred.storeData(ssid, passw);
        network.resetWifi();
    };

    Serv_Device servDev;
    Mng_Network network;
    Mng_Power power;
    BNT_Hold releasedState = HOLD_TRANSITION;

    public:
        Mng_Device(): BaseComponent("MngDev") {}

        Mng_Runtime runtime;
        // void showLadderId() override {
        //     servDev.i2c.addDisplayQueue("ladderId = " + String(network.tweet.attendant.ladderId), 0);
        // }

        void setup() {
            xLog(__func__);            
            //! setupNetwork first to save time connecting
            servDev.configure();
            servDev.button1.setup(servDev.P_CONF.btn1, &buttonCb);
            servDev.irSwitch.load(servDev.P_CONF.irSwitch, &irSwitchCb);
            servDev.edgeDetector.setup(servDev.P_CONF.pir1, &pirCb);
            servDev.serial.onStoreCred = &storeCredCb;

            network.setupNetwork(&servDev);
            runtime.setup(&servDev, &network);
            AppPrintHeap();
        }
};

// ESPNow, behavior
// PIR Multi
// Lora Bluetooth, mqtt
// FS update, sd storage
// device encryption
// device discovery

// multi channel
// - Storing channel
// - Scan channel
// - broadcast channel
