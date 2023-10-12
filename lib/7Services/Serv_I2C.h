enum DisplayMode {
    DISPLAY_2ND,
    DISPLAY_DEFAULT
};

struct DispItem {
    char data[22];
    uint8_t line;

    bool isEmpty() { return strcmp(data, "") == 0; }

    void makeEmpty() {
        strcpy(data, "");
    }

    static DispItem make(const char* str, uint8_t line) {
        DispItem output;
        output.line = line;
        memcpy(output.data, str, sizeof(data));
        return output;
    }
};

class Serv_I2C: public Loggable {
    public:
        Serv_I2C(): Loggable("I2C") {}

        Disp_SSD13062 disp;
        DisplayMode dispMode = DISPLAY_DEFAULT;
        Mng_Sensor sensors;
        Mod_RTC rtc;
        PCA96Controller pca96;

        bool isLoaded = false;
    
        void setup(int scl, int sda, TwoWire *wire = &Wire) {
            bool error = scl == 255 || sda == 255;
            xLogStatus(__func__, !error);
            if (error) return;

            wire->begin(sda, scl);
            sensors.setup(wire);
            rtc.setup(wire);
            pca96.setup(wire);
            disp.setup(wire);
            disp.printline("Hello World!", 0);
            isLoaded = true;
        }
        
        void switchDisplayMode() {
            xLog(__func__);
            dispMode = (dispMode == DISPLAY_DEFAULT) ? DISPLAY_2ND : DISPLAY_DEFAULT;
            disp.clear();
        }

        //! Run
        void run() {
            // sensors.checkStatus();
            // pca96.test();
        }
};