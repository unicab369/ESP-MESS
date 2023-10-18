#include <IRremote.h>
#define IR_RECEIVE_THRESDHOLD 50

class IRSwitch {
    IRsend irSender;
    uint8_t pin;
    unsigned long timeRef = 0;

    public:
        std::function<void(bool, uint32_t)> *callback;
        uint16_t receiveCounter = 0;

        void load(uint8_t _pin) {
            Serial.print("[IR] "); Serial.println(__func__);
            if (_pin == 255) { return; }
            pin = _pin,
            resetIR();
        }

        void resetIR() {
            receiveCounter = 1;
            timeRef = 0;
            IrReceiver.begin(pin, ENABLE_LED_FEEDBACK);
        }

        void run() {
            if (receiveCounter == 0 || callback == nullptr || IrReceiver.decode() == false) return;
            (*callback)(true, IrReceiver.decodedIRData.decodedRawData);
            IrReceiver.resume();

            if (receiveCounter > IR_RECEIVE_THRESDHOLD) {
                unsigned long timeDif = millis()-timeRef;

                if (timeDif>1000) {
                    (*callback)(false, 0);
                    IrReceiver.stop();
                    receiveCounter = 0;
                } else {
                    timeRef = millis();
                    receiveCounter = 1;     // reset to active counting
                }
            } else if (timeRef == 0) {
                timeRef = millis();
            }

            receiveCounter++;   // ORDER DOES MATTER
        }

        void send(uint32_t data, uint8_t len = 32) {
            irSender.sendNEC(data, len);
        }
};