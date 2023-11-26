#define MAX_DISPLAY_QUEUE1 8
#define MAX_DISPLAY_QUEUE2 20


class Serv_Serial: public Loggable {
    AppQueue<DispItem, MAX_DISPLAY_QUEUE1> dispQueue1;
    AppQueue<DispItem, MAX_DISPLAY_QUEUE2> dispQueue2;

    Disp_ST77 largeDisp;
    Disp_Epaper epaper;

    public:
        Serv_I2C i2c1;
        Serv_I2C i2c2;
        Mng_Storage storage;
        Mng_AppClock appClock;
        Mod_RTC1302 rtc2;

        Serv_Serial(): Loggable("Serial") {}

        void setupSerial(Serv_IO* conf) {
            xLogSection(__func__);
            
            rtc2.setup();

            //! setup i2C
            if (conf->checkWire0()) {
                i2c1.setup(conf->scl0, conf->sda0, &Wire);    //! ORDER DOES MATTER
                appClock.setup(&(i2c1.rtc));                    //! ORDER DOES MATTER
            }

            #ifdef ESP32
                // pinMode(14, OUTPUT);
                pinMode(36, INPUT);
                pinMode(39, INPUT);
                i2c2.setup(conf->scl1, conf->sda1, &Wire1);
            #endif

            storage.setupStorage();

            if (conf->checkHSPIPins() && conf->out3 != 255) {
                MY_ESP.printSPIPins();

                appTimer0.logElapse("LogTime until Storage");

                #ifdef ESP32
                    SPI.begin(conf->sck0, conf->miso0, conf->mosi0);
                #else
                    SPI.begin();
                #endif
                storage.setupSDCard(conf->out0);

                pinMode(conf->out3, OUTPUT);
                digitalWrite(conf->out3, HIGH);
                largeDisp.setup2(conf->out1, conf->out2, conf->rst0, conf->mosi0, conf->sck0);
                // epaper.setup();
            }
        }

        //! 1 Second Interval
        void render1s_Interval(AsyncTimer* aTimer1, AsyncTimer* aTimer2, std::function<void()> handleDisplayMode) {
            bool checkConn = i2c2.ch32v.checkConnection();
            xLogf("I2C1 Connection = %d", checkConn);
            // i2c2.ch32v.requestReadings();
            // rtc2.run();
            // String timeStr = i2c1.rtc.timeDisplay();
            // // xLogf("TimeStr = %s", timeStr.c_str());
            // Serial.println(timeStr);
            // i2c1.as5600.run();
            // epaper.printLn();

            if (i2c1.dispMode == DISPLAY_DEFAULT) {
                aTimer1->model.isEven ? i2c1.sensors.requestReadings() : i2c1.sensors.collectReadings();

                addDisplayQueue1(appClock.getDisplay(), 1);         //* LINE 1   
                addDisplayQueue1(aTimer1->record(), 2);             //* LINE 2
                addDisplayQueue1(aTimer2->record(), 3);             //* LINE 3
                addDisplayQueue1(i2c1.sensors.getTempHumLux(), 5);  //* LINE 5
                // i2c2.sen

            } else if (i2c1.dispMode == DISPLAY_2ND) {
                handleDisplayMode();
            }
        }

        //! 5 Seconds Interval
        void render5s_Interval(const char* hostName) {
            char sdSize[22];
            sprintf(sdSize, "sd = %u MB", storage.sd1.getCardSize());

            if (i2c1.dispMode == DISPLAY_DEFAULT) {
                addDisplayQueue1(hostName, 0);     //! Oled Mini LINE 0  
                addDisplayQueue1(sdSize, 4);       //! Oled Mini LINE 4  
                addDisplayQueue1("Heap: " + String(ESP.getFreeHeap()), 6);      //* LINE 6
            }
        }

        //! 3 Seconds Interval
        void render3s_Interval(AsyncTimer* aTimer1, AsyncTimer* aTimer2) {
            char output[22];
            sprintf(output, "dep = %u/%u", aTimer1->stackUsage, aTimer2->stackUsage);

            if (i2c1.dispMode == DISPLAY_DEFAULT) {
                // showLadderId(); // line0
                addDisplayQueue1(String(output), 4);   //! Oled Mini LINE 4
            }
            addDisplayQueue2(output, 4);               //* LINE 4
        }

        //! 2 Seconds Interval
        void render2s_Interval(String localIP) {
            char heapInfo[22];
            char networkInfo[64];
            sprintf(heapInfo, "mem = %u/%u", MY_ESP.maxHeap(), ESP.getFreeHeap());

            uint64_t resetCount = storage.stoStat.resetCnt();
            sprintf(networkInfo, "%s ~%u ~%llu", localIP, WiFi.channel(), resetCount);

            //! Oled Mini
            if (i2c1.dispMode == DISPLAY_DEFAULT) {
                addDisplayQueue1(networkInfo, 0);      //! Oled Mini LINE 0
                addDisplayQueue1(heapInfo, 4);   //! Oled Mini LINE 4        
            }
            addDisplayQueue2(networkInfo, 0);          //* LINE 0
            addDisplayQueue2(heapInfo, 4);               //* LINE 4 
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
            // storage.addStoreTempHumLuxQueue(temp, hum, lux, appClock.getTimeNow());
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
};