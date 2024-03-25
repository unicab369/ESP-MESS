#define MAX_DISPLAY_QUEUE1 8
#define MAX_DISPLAY_QUEUE2 20
#define MAX_PLOTTER_QUEUE 10

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
            storage.setupStorage();

            //! setup i2C
            if (conf->checkWire0()) {
                i2c1.setup(conf->scl0, conf->sda0, &Wire);    //! ORDER DOES MATTER
                appClock.setup(&(i2c1.rtc));                    //! ORDER DOES MATTER
            }

            #ifdef ESP32
                if (conf->checkWire1()) {
                    i2c2.setup(conf->scl1, conf->sda1, &Wire1);
                }
            #endif

            if (conf->checkHSPIPins()) {
                MY_ESP.printSPIPins();
                appTimer0.logElapse("LogTime until Storage");

                #ifdef ESP32
                    SPI.begin(conf->sck0, conf->miso0, conf->mosi0);
                #else
                    SPI.begin();
                #endif
                storage.setupSDCard(conf->cs1);

                largeDisp.setup2(conf->cs0, conf->ao0, conf->rst0, conf->mosi0, conf->sck0);
                // epaper.setup();
            }
        }

        //! DisplayQueue1
        void addDisplayQueue1(const char* str, uint8_t line) {
            if (!i2c1.isLoaded) return;
            DispItem item = DispItem::make(str, line);
            dispQueue1.sendQueue(&item);   
        }

        //! DisplayQueue2
        void addDisplayQueue2(const char* str, uint8_t line) {
            if (!largeDisp.isLoaded) return;
            DispItem item = DispItem::make(str, line);
            dispQueue2.sendQueue(&item);            
        }

        void addDataStoreQueue(DataStoreItem *item) {
            if (!storage.isValidPath()) return;
            storage.storeItem(item);
        }

        //! store and reset sensors readings
        void addStoreQueue() {
            if (!storage.isValidPath()) return;
            float temp, hum, lux;
            i2c1.sensors.getTempHumLux(&temp, &hum, &lux);

            DataStoreItem item;
            item.val1 = temp;
            item.val2 = hum;
            item.val3 = lux;
            item.timeStamp = appClock.getTimeNow();
            storage.storeItem(&item);
        }

        //! Handle Qeueues, one at a time
        void handleQueues() {
            DispItem item;

            //! Finish dispQueue1 first
            if (dispQueue1.getQueue(&item)) {
                i2c1.disp.printline(item.data, item.line);
            } 
            
            //! Finish dispQueue2 next
            else if (dispQueue2.getQueue(&item)) {
                largeDisp.printline(item.data, item.line);
            }
            
            else {
                //! handle valueQueue
                storage.handleValueQueue([&](uint32_t writeTime) {
                    char output[22];
                    sprintf(output, "sd wr: %lu ms", writeTime);
                    addDisplayQueue2(output, 8);
                });
            }
        }


        bool handleConsoleCmd(char* inputStr) {
            //# WARNING: strtok detroys the original string, perform operation on copied string
            char clonedStr[128] = "";
            memcpy(clonedStr, inputStr, sizeof(clonedStr));
            char *ref = strtok(inputStr, " ");

            //! check keys
            //! SHT=0x44, SSD13=0x3D, BH17=0x23
            if (strcmp(ref, "scani2c1") == 0) {
                i2c1.scanAll();
                bool check = i2c1.ch32v.checkConnection();
                xLogf("CHECK CONN = %u", check);
                i2c1.ch32v.requestReadings(); 

            } else if (strcmp(ref, "scani2c2") == 0) {
                i2c2.scanAll();
                bool check = i2c2.ch32v.checkConnection();
                xLogf("CHECK CONN = %u", check);
                i2c2.ch32v.requestReadings();
                // i2c2.disp.printline("ZZZZZZZZZZZZ", 0);
                // i2c2.disp.printline("YYYYYYYYYYYY", 1);
            }

            return true;
        }

        void tick() {
            i2c1.run();
            handleQueues();         //! handle DisplayQueues
        }
};