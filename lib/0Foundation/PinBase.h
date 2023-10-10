struct PinsConf {
    uint8_t irSwitch = 255;
    uint8_t ws2812 = 255;
    uint8_t buzzer1 = 255;
    uint8_t led1 = 255; uint8_t led2 = 255;
    uint8_t pir1 = 255; uint8_t pir2 = 255;
    uint8_t btn1 = 255; uint8_t btn2 = 255;
    uint8_t relay1 = 255; uint8_t relay2 = 255;
    
    //! i2C lines
    uint8_t sda0 = 255; uint8_t scl0 = 255;
    uint8_t sda1 = 255; uint8_t scl1 = 255;

    //! SPI lines
    uint8_t mosi0 = 255, miso0 = 255, sck0 = 255, rst0 = 255;
    uint8_t out0 = 255, out1 = 255, out2 = 255, out3 = 255;

    uint8_t mosi1 = 255, miso1 = 255, sck1 = 255, rst1 = 255;

    uint8_t cs1 = 255; uint8_t di01 = 255;
    uint8_t swRx = 255; uint8_t swTx = 255;
    uint8_t rotaryA = 255; uint8_t rotaryB = 255;

    bool checkHSPIPins() {
        return sck0 != 255 && miso0 != 255 && mosi0 != 255;
    }

    bool checkWire0() {
        return sda0 != 255 && scl0 != 255;
    }
};

class PinReadable {
    uint8_t pin = 255;
    bool prevRead = false;

    public:
        uint8_t get_pin()   { return pin; }           // return the stored pin
        bool isValid()      { return pin != 255; }       // check if pin is valid
        bool isOn()         { return pin_read() == 1; }
        bool isOff()        { return pin_read() == 0; }
        bool previousRead() { return prevRead; }
        
        // setup - keep track of pin
        bool pin_setup(uint8_t _pin, bool inputPullup = false) {
            //! dont use ternary operator here
            if (inputPullup) {
                pinMode(_pin, INPUT_PULLUP);
            } else {
                pinMode(_pin, INPUT);
            }
            pin = _pin;
            return _pin != 255;
        }

        // read the pin
        bool pin_read() {
            if (!isValid()) { return false; }
            return digitalRead(get_pin());
        }

        // check if pin is different than `bool read` input
        bool compareToPrevRead(bool compareValue) {
            bool changed = (compareValue != prevRead);
            prevRead = compareValue;
            return changed;
        }

        bool compareToCurrentRead(bool compareValue) {
            return pin_read() == compareValue;
        }

        // check if pin is different than current pin_read()
        bool hasChanged() {
            return compareToPrevRead(pin_read());
        }
};

class PinWritable: public PinReadable {
    bool inverted = false;

    public:
        PinWritable(): PinReadable() {}

        // setup - store pin, set pin as OUTPUT
        bool setup(uint8_t pin, bool _inverted = false) {
            if (!pin_setup(pin)) { return false; }
            inverted = _inverted;
            pinMode(pin, OUTPUT);
            return true;
        }

        void turnOn() { pin_write(HIGH); }
        void turnOff() { pin_write(LOW); }

        // write value to pin
        void pin_write(bool value) {
            if (!isValid()) { return; }
            digitalWrite(get_pin(), inverted ? !value : value);
        }

        // toggle the current pin state
        void toggle() {
            if (!isValid()) { return; }
            digitalWrite(get_pin(), !pin_read());
        }
};

class EdgeDetector: private PinReadable {
    TimeoutItem cooldown_timer;
    uint32_t timeRef;
    uint32_t retriggerTimeout;        // trigger again on active HIGH
    bool didReset = false;
    std::function<void(bool, uint8_t)> *callback;

    public:
        EdgeDetector(): PinReadable() {}
        
        void setup(uint8_t _pin, std::function<void(bool, uint8_t)> *cb, 
                uint32_t cooldown = 1000, uint32_t retrigger = 5000) {
            callback = cb;
            retriggerTimeout = retrigger;
            cooldown_timer.load(cooldown);
            pin_setup(_pin);
        }

        void run() {
            if (isValid() == false || !callback) { return; }
            bool read = pin_read();

            if (compareToPrevRead(read)) {
                // if (read) { timeRef = millis(); }
                // else { Serial.print("\nhigh to low: "); Serial.println(millis()-timeRef); }

                // if reading changed: 
                if (read && (didReset || cooldown_timer.check())) {
                    cooldown_timer.reset();
                    // Serial.print("\n[pir] Triggered: " + String(get_pin()) + " | 1"); 
                    (*callback)(read, get_pin());
                    didReset = false;

                } else {
                    // Serial.print("\n[pir] Triggered: " + String(get_pin()) + " | 0");
                    (*callback)(read, get_pin());
                    didReset = false;
                }

            } else if (!didReset) {
                // if reading not change: either reset or retrigger
                if (read && cooldown_timer.update(retriggerTimeout)) {
                    // Serial.println("\n[Pir] ReTrigger " + String(get_pin()));
                    (*callback)(read, get_pin());         // retrigger if active HIGH

                } else if (!read && cooldown_timer.check()) {
                    didReset = true;  // reset if havent read HIGH for a cooldown time
                }
            }
        }
};

typedef std::function<void(bool, uint8_t, bool)> Multi_EdgeDetector_Callback;

#define EDGE_DETECTOR_MAX 5

class Multi_EdgeDetectors {
    TimeoutItem groupTimeout;
    EdgeDetector detectors[EDGE_DETECTOR_MAX];
    Multi_EdgeDetector_Callback callback;

    public:
        Multi_EdgeDetectors() {}

        void setup(Multi_EdgeDetector_Callback _cb, unsigned long groupCooldown = 10000) {
            callback = _cb;
            groupTimeout.load(groupCooldown);
        }

        void load(uint8_t index, uint8_t pin, unsigned long _cooldown = 3000, 
            unsigned long _retrigger = 5000) {
            if (index+1 > EDGE_DETECTOR_MAX || callback == NULL) { return; }

            // detectors[index].begin(pin, [this](bool state, uint8_t pin) {
            //     callback(state, pin, groupTimeout.check(true));
            // }, _cooldown, _retrigger);
        }

        void run() {
            if (callback == NULL) { return; }
            for (int i=0; i<EDGE_DETECTOR_MAX; i++) {
                detectors[i].run();
            }
        }
};