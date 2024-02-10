byte BH17_POWER_DOWN[1] = { 0x00 };
byte BH17_POWER_ON[1]   = { 0x01 };
byte BH17_RESET[1]      = { 0x07 };

byte BH17_CONT_HI1[1]   = { 0x10 };   // 1 lux resolution 120ms
byte BH17_CONT_HI2[1]   = { 0x11 };   // .5 lux resolution 120ms
byte BH17_CONT_LOW[1]   = { 0x13 };   // 4 lux resolution 16ms
byte BH17_ONCE_HI1[1]   = { 0x20 };   // 1 lux resolution 120ms
byte BH17_ONCE_HI2[1]   = { 0x21 };   // .5 lux resolution 120ms
byte BH17_ONCE_LOW[1]   = { 0x23 };   // 4 lux resolution 16ms

byte BH17_BEGIN[2]      = { 0x01, 0x23 };
#define BH17_DEFAULT_MTREG 69

class Mod2_BH17: public SensorBase, public Lux_Interface {
    
    void onReceiveData(uint16_t *buf) override {
        uint32_t value = buf[0];
        value <<= 8;
        value |= buf[1];
        if (value>10000000) { value = 0; }
        setLux(value/1.2);
    }

    public:
        //! addr 0x23
        Mod2_BH17(): SensorBase(0x23), Lux_Interface() {}

        uint16_t setup(TwoWire *wire) override {
            uint16_t value = _setup(wire, BH17_CONT_LOW, 1, 0);
            return value;
        }

        void reset() override { 
            setLux(-1); 
        }

        bool requestReadings() override {
            return _requestReadings(0, 0, 10, 2);
        }
};

class Mod_Ch32v003: public SensorBase {
    public:
        //! addr 0xfa
        Mod_Ch32v003(): SensorBase(0x78) {}

        uint16_t setup(TwoWire *wire) override {
            uint16_t value = _setup(wire, BH17_POWER_ON, 1);
            return value;
        }

        uint8_t value = 0;

        bool requestReadings() override {
            thisWire->requestFrom(0x78, 10);
            uint8_t buf[10] = { 0 };

            Serial.println("***READ=");
            for (int i=0; i<10; i++) {
                buf[i] = thisWire->read();
                Serial.printf("%u, ", buf[i]);
            }
            Serial.println();

            thisWire->beginTransmission(0x78);
            thisWire->write(0x00);          // register location?
            thisWire->write(0x05);          // register new value?
            int err = thisWire->endTransmission();
            Serial.printf("\n*** SENSOR ERROR = %u", err);

            return false;
        }
};