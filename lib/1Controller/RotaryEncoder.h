enum RotaryDirection: int8_t {
   NOROTATION = 0,
   CLOCKWISE = 1,
   COUNTERCLOCKWISE = -1
};

class RotaryEncoder: public Loggable {
   PinReadable pinA;
   PinReadable pinB;
   uint16_t counter = 0;
   uint8_t increment = 0;
   RotaryDirection rotaryState = NOROTATION;
   TimerItem timer;
   MinMax minMax;

   public:
      bool isEnabled = false;
      // std::function<void(RotaryDirection, uint16_t)> onCallback = [](RotaryDirection, uint16_t){};

      RotaryEncoder(): Loggable("Rotary") {}

      void setup(uint8_t _pinA, uint8_t _pinB, uint8_t _increment = 1,
               MinMax _minMax = {.min = 0, .max = 4096}) {
         bool check = pinA.pin_setup(_pinA, true) && pinB.pin_setup(_pinB, true);
         xLogStatus(__func__, check);
         if (!check) return;

         increment = _increment;
         minMax = _minMax;
         isEnabled = true;
      }

      void run(std::function<void(RotaryDirection, uint16_t)> onCallback) {
         if (!pinA.hasChanged() || !isEnabled) return;
         rotaryState = (pinA.compareToCurrentRead(pinB.pin_read())) ? COUNTERCLOCKWISE : CLOCKWISE;
         counter += (rotaryState*increment);

         if (counter < minMax.min) {
            counter = minMax.min;
         } else if (counter > minMax.max) {
            counter = minMax.max;
         }

         timer.onTimeoutWithReset(100, [&]() {
            onCallback(rotaryState, counter); 
         });
      }
};

