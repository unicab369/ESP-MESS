enum RotaryDirection: int8_t {
    NOROTATION = 0,
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = -1
};

class RotaryEncoder: private PinReadable {
    uint8_t pinB;
    uint8_t counter = 0; 
    RotaryDirection rotaryState = NOROTATION;
    TimerItem timer;

    public:
        bool isEnabled = false;
        std::function<void(RotaryDirection, uint8_t)> *onCallback;

        void setup(uint8_t _pinA, uint8_t _pinB) {
            if (_pinA == 255 || _pinB == 255) return;
            pinB = _pinB;
            pinMode(pinB, INPUT_PULLUP);
            pin_setup(_pinA, true);
        }

        void run() {
            if (onCallback == nullptr || !hasChanged() || !isEnabled) return;
            rotaryState = (compareToCurrentRead(digitalRead(pinB))) ? COUNTERCLOCKWISE : CLOCKWISE;
            counter += rotaryState;

            timer.onTimeoutWithReset(100, [&]() {
                (*onCallback)(rotaryState, counter); 
            });
        }
            
};

