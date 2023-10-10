// #define TTGO_LORA32 true

// #define MAX_DISPLAY_QUEUE1 8
// #define MAX_DISPLAY_QUEUE2 20

// #define DEV_KMC_70011 1

class Serv_Device: public BaseComponent, public Serv_Serial {

    std::function<void(bool, uint32_t)> irSwitchCb = [&](bool status, uint32_t value) {
        String output = "IrRead = " + (status ? String(value) : "Locked");
        AppPrint("[IR]", output);
        addDisplayQueue1(output, 6);
    };

    std::function<void(bool, uint8_t)> pirCb = [&](bool status, uint8_t pin) {
        Serial.print("PirTriggered = "); Serial.print(pin);
        Serial.print(" | "); Serial.println(status);
    };

    BNT_Hold releasedState = HOLD_TRANSITION;

    std::function<void(BTN_Action, BNT_Hold, uint32_t)> buttonCb = 
                        [&](BTN_Action action, BNT_Hold hold, uint32_t elapse) {
        switch (action) {
            case ACTION_SINGLE_CLICK: {
                xSerial.println("Hellow there\n");
                addDisplayQueues("Single Click", 6);
                // network.handleSingleClick();
                if (onHandleSingleClick) (*onHandleSingleClick)();
                toggleRelay();

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
                addDisplayQueues("Double Click", 6);
                // servDev.i2c1.switchDisplayMode();
                // network.handleDoubleClick();
                if (onHandleDoubleClick) (*onHandleDoubleClick)();
                break;
            }
            case ACTION_PRESS_ACTIVE: {
                //! Long press 20 sec -> Recovery/Factory Reset - turn off
                if (hold == HOLD_20_SEC) {
                    addDisplayQueues("[Bnt] 20sec hold", 6);
                    led.setOFF();
                    storage.deleteData();

                //! Long press 10 sec -> Reset - fash flashing
                } else if (hold == HOLD_10_SEC) {
                    addDisplayQueues("[Bnt] 10sec hold", 6);
                    led.repeatPulses(400);
                    releasedState = hold;

                //! Long press 5 sec -> Pairing - double flashing
                } else if (hold == HOLD_5_SEC) {
                    addDisplayQueues("[Bnt] 5sec hold", 6);
                    led.repeatPulses(20);
                    releasedState = hold;
                }
                break;
            }
            case ACTION_PRESS_END: {
                addDisplayQueues("Press Ended " + String(elapse), 6);
                if (releasedState == HOLD_5_SEC) {
                    MY_ESP.restart();           //! Restart Device
                } else if (releasedState == HOLD_10_SEC) {
                    // network.startAP(true);      //! Start access point
                    if (onHandleAPRequest) (*onHandleAPRequest)();
                }
                break;
            }
        }
    };

    
    public:
        Serv_Device(): BaseComponent("Dev"), Serv_Serial() {}

        WS28xx ws2812;
        MyButton button1;
        PinWritable pinWrite;
        IRSwitch irSwitch;
        // SerialControl serial;
        EdgeDetector edgeDetector;
        PinsConf P_CONF;
        ExtraSerial xSerial;

        OutputController led, buzzer;
        PinWritable relay1;

        std::function<void()> *onHandleSingleClick;
        std::function<void()> *onHandleDoubleClick;
        std::function<void()> *onHandleAPRequest;

        virtual void showLadderId() {}

        void configure() {
            #ifdef TTGO_LORA32
                AppPrintSeparator("[DIRECTIVE]", "TTGO_LORA32");
                // pinMode(16, OUTPUT);
                // digitalWrite(16, HIGH);
                // delay(20);

                // pinMode(17, INPUT_PULLUP);
                // pinMode(2, OUTPUT);
                // P_CONF.sda0 = 4; P_CONF.scl0 = 15;
                P_CONF.miso1 = 19; P_CONF.mosi1 = 23; P_CONF.sck1 = 18;  P_CONF.cs1 = 5;
                P_CONF.di01 = 2; P_CONF.rst1 = 14;

                vspi->begin(P_CONF.sck1, P_CONF.miso1, P_CONF.mosi1, P_CONF.cs1);
                SPI.begin(P_CONF.sck1, P_CONF.miso1, P_CONF.mosi1, P_CONF.cs1);
                // lora.setup(P_CONF.cs1, P_CONF.rst1, P_CONF.di01, *vspi);
                
                // lora.loadReceiveCb([this](Lora_Packet *packet) {
                //     Serial.println("LOra received"); 
                //     packet->print();
                //     // i2c.disp.printline("Lora: " + String(packet->data0), 5);
                //     // digitalWrite(2, !digitalRead(2));
                // });

            #elif defined(DEV_KMC_70011)
                P_CONF.led1 = 13;
                P_CONF.btn1 = 0;
                P_CONF.relay1 = 14;

            #elif ESP32
                AppPrintSeparator("[DIRECTIVE]", "ESP32");
                // use for disp0
                // pinMode(5, OUTPUT);
                // pinMode(18, OUTPUT);
                // digitalWrite(5, LOW);
                // digitalWrite(18, HIGH);

                // P_CONF.ws2812 = 12;
                // P_CONF.led1 = 22;
                // P_CONF.sda0 = 32; P_CONF.scl0 = 33;
                // P_CONF.pir1 = 36;
                // P_CONF.irSwitch = 26;
                // P_CONF.btn1 = 23;
                // P_CONF.buzzer1 = 19;

                P_CONF.ws2812 = 12;
                P_CONF.led1 = 22;
                P_CONF.btn1 = 34;
                P_CONF.buzzer1 = 14;
                P_CONF.rotaryA = 13; P_CONF.rotaryB = 15;
                // P_CONF.pir1 = 36;
                // P_CONF.irSwitch = 26;
                // P_CONF.swRx = 0; P_CONF.swTx = 2;
                
                //! i2C Pins
                P_CONF.sda0 = 33; P_CONF.scl0 = 32;
                P_CONF.sda1 = 4; P_CONF.scl1 = 0;

                //! SPI Pins
                P_CONF.sck0 = 18;       //! SCK
                P_CONF.mosi0 = 23;      //! MOSI
                P_CONF.miso0 = 19;      //! MISO
                P_CONF.rst0 = 2;        //! RST
                
                P_CONF.out0 = 5;        // sdCard CS
                P_CONF.out1 = 27;       // display CS
                P_CONF.out2 = 26;       // display AO
                P_CONF.out3 = 25;       // display backlight

            #else
                xLogSection("[DIRECTIVE] ESP8266");

                // AppPrintSeparator("[DIRECTIVE]", "ESP8266");
                // D0: 16, D1: 5, D2: 4, D3: 0, D4: 2, D5: 14, D6: 12, D7: 13, D8: 15
                // P_CONF.sda0 = 4; P_CONF.scl0 = 5;
                P_CONF.led1 = 2;
                P_CONF.btn1 = 0;
                P_CONF.relay1 = 14;
                // P_CONF.ws2812 = 15;
                // P_CONF.irSwitch = 12;
                // P_CONF.pir1 = 14;

                // SPI.pins(14, 12, 13, 15);
                // SPI.begin();
                // P_CONF.swRx = 14; P_CONF.swTx = 12;
            #endif
        
            led.setup(P_CONF.led1);
            led.repeatPulses(1000);
            relay1.setup(P_CONF.relay1);

            // buzzer.setup(P_CONF.buzzer1);
            ws2812.setup(P_CONF.ws2812);
            // xSerial.setup(P_CONF.swRx, P_CONF.swTx);

            setupSerial(&P_CONF);

            irSwitch.load(P_CONF.irSwitch, &irSwitchCb);
            edgeDetector.setup(P_CONF.pir1, &pirCb);
            button1.setup(P_CONF.btn1, &buttonCb);
        }

        void toggleRelay() {
            xLog("IM HERE 222");
            relay1.toggle();
        }

        void runGroupTasks() {
            button1.run();
            irSwitch.run(); 
            led.run();
            ws2812.run();
            serial.run();
            
            // buzzer.run();
            // edgeDetector.run();
            // xSerial.run();
        }

        void setFrequency() {
            #ifdef ESP32
                // system_update_cpu_freq(80);
            #else
                
            #endif
        }
};