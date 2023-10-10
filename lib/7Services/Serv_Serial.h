#define MAX_DISPLAY_QUEUE1 8
#define MAX_DISPLAY_QUEUE2 20

class Serv_Serial {
    AppQueue<DispItem, MAX_DISPLAY_QUEUE1> dispQueue1;
    AppQueue<DispItem, MAX_DISPLAY_QUEUE2> dispQueue2;

    Disp_ST77 largeDisp;

    std::function<void(const char* ssid, const char *passw)> 
            storeCredCb = [&](const char* ssid, const char *passw) {
        storage.stoCred.storeData(ssid, passw);
        if (onHandleResetWifi) (*onHandleResetWifi)();
    };

    public:
        Serv_I2C i2c1;
        Serv_I2C i2c2;
        SerialControl serial;
        Mng_Storage storage;
        Mng_AppClock appClock;

        std::function<void()> *onHandleResetWifi;

        void setupSerial(PinsConf* conf) {
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

            storage.setup();

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
            }

            serial.onStoreCred = &storeCredCb;
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
};