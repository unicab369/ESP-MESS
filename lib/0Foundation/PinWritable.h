#include "PinReadable.h"

class PinWritable: public PinReadable {
    bool inverted = false;

    public:
        PinWritable(): PinReadable() {}

        // setup - store pin, set pin as OUTPUT
        bool begin(uint8_t pin, bool _inverted = false) {
            if (!pin_setup(pin)) { return false; }
            inverted = _inverted;
            pinMode(pin, OUTPUT);
            return true;
        }

        virtual void turnON() { pin_write(HIGH); }
        virtual void turnOFF() { pin_write(LOW); }

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

class PWMWritable: public PinWritable {
    int refValue = 0;
    bool inverted = false;
    const int ledChannel = 0;
    const int resolution = 8;
    const int freq = 5000;

    public:
        void setup(uint8_t pin, bool _inverted = false) {
            inverted = _inverted;
            begin(pin, inverted);

            #ifdef ESP32
                ledcSetup(ledChannel, freq, resolution);
                ledcAttachPin(pin, ledChannel);
            #endif
        }

        void pwmWrite(uint8_t value) {
            // refValue = value;
            uint8_t writeValue = inverted ? (255-value) : value;
            
            #ifdef ESP32
                ledcWrite(ledChannel, value);
            #else
                analogWrite(get_pin(), value);
            #endif
        }

        void fadeToward(int newValue, int change) {
            if (refValue>newValue) {
                Serial.print("+refVal="); Serial.print(refValue);
                Serial.print(" newVal="); Serial.println(newValue);
                refValue += change;
                refValue = refValue > newValue ? newValue : refValue;
            } else if (newValue > refValue) {
                Serial.print("-refVal="); Serial.print(refValue);
                Serial.print(" newVal="); Serial.println(newValue);
                refValue -= change;
                refValue = refValue < newValue ? newValue : refValue;
            }
            // Serial.print("value="); Serial.println(refValue);
            pwmWrite(refValue);
        }
};