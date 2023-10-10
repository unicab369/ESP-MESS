// #define TTGO_LORA32 true

#define MAX_DISPLAY_QUEUE1 8
#define MAX_DISPLAY_QUEUE2 20

// #define DEV_KMC_70011 1

class Serv_Device: public BaseComponent {
    AppQueue<DispItem, MAX_DISPLAY_QUEUE1> dispQueue1;
    AppQueue<DispItem, MAX_DISPLAY_QUEUE2> dispQueue2;

    public:
        Serv_Device(): BaseComponent("Dev") {}

        WS28xx ws2812;
        MyButton button1;
        PinWritable pinWrite;
        IRSwitch irSwitch;
        Disp_ST77 largeDisp;
        SerialControl serial;
        EdgeDetector edgeDetector;
        PinsConf P_CONF;
        ExtraSerial xSerial;

        Serv_I2C i2c1;
        Serv_I2C i2c2;
        Mng_Storage storage;
        OutputController led, buzzer;
        PinWritable relay1;
        Mng_AppClock appClock;
        
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

            //! setup i2C
            if (P_CONF.checkWire0()) {
                i2c1.setup(P_CONF.scl0, P_CONF.sda0, &Wire);    //! ORDER DOES MATTER
                appClock.setup(&(i2c1.rtc));                    //! ORDER DOES MATTER
            }

            #ifdef ESP32
                // pinMode(14, OUTPUT);
                pinMode(36, INPUT);
                pinMode(39, INPUT);
                i2c2.setup(P_CONF.scl1, P_CONF.sda1, &Wire1);
            #endif

            storage.setup();

            if (P_CONF.checkHSPIPins() && P_CONF.out3 != 255) {
                MY_ESP.printSPIPins();

                appTimer0.logElapse("LogTime until Storage");

                #ifdef ESP32
                    SPI.begin(P_CONF.sck0, P_CONF.miso0, P_CONF.mosi0);
                #else
                    SPI.begin();
                #endif
                storage.setupSDCard(P_CONF.out0);

                pinMode(P_CONF.out3, OUTPUT);
                digitalWrite(P_CONF.out3, HIGH);
                largeDisp.setup2(P_CONF.out1, P_CONF.out2, P_CONF.rst0, P_CONF.mosi0, P_CONF.sck0);
            }
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

        //! DisplayQueue1
        void addDisplayQueue1(String str, uint8_t line) {
            if (!i2c1.isLoaded) return;
            const char* arr = str.c_str();
            DispItem item = DispItem::make(arr, line);
            dispQueue1.sendQueue(&item);   
        }

        //! DisplayQueue2
        void addDisplayQueue2(String str, uint8_t line) {
            if (!largeDisp.isLoaded) return;
            const char* arr = str.c_str();
            DispItem item = DispItem::make(arr, line);
            dispQueue2.sendQueue(&item);            
        }

        //! Add to both DisplayQueue1 and DisplayQueue2
        void addDisplayQueues(String str, uint8_t line) {
            addDisplayQueue1(str, line);
            addDisplayQueue2(str, line);
        }

        //! store and reset sensors readings
        void addStoreQueue() {
            if (!storage.isValidPath()) return;
            float temp, hum, lux;
            i2c1.sensors.getTempHumLux(&temp, &hum, &lux);
            storage.addStoreTempHumLuxQueue(temp, hum, lux, appClock.getTimeNow());
        }

        //! Handle Qeueues
        void handleQueues() {
            DispItem item;
            //! Handle one at a time
            if (dispQueue1.getQueue(&item)) {
                i2c1.disp.printline(String(item.data), item.line);
            } 
            //! Finish dispQueue1 first
            else if (dispQueue2.getQueue(&item)) {
                largeDisp.printline(String(item.data), item.line);
            }
            //! Finish dispQueue2 first
            else {
                //! handle valueQueue
                storage.handleValueQueue([&](uint32_t writeTime) {
                    addDisplayQueue2("sd write: " + String(writeTime) + "ms", 8);
                });
            }
        }

        void tick() {
            i2c1.run();
            handleQueues();         //! handle DisplayQueues
        }

        void setFrequency() {
            #ifdef ESP32
                // system_update_cpu_freq(80);
            #else
                
            #endif
        }
};