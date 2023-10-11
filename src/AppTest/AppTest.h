#ifdef ESP32 
    uint8_t ledPin = 22;
#else
    uint8_t ledPin = 2;
#endif

#define TEST_PWM True

#ifdef TEST_BLINK
    void setup() {
        Serial.begin(115200);
        AppPrint("IM HERE 000");
        pinMode(ledPin, OUTPUT);
    }

    void loop() {
        AppPrint("IM HERE 111");
        uint8_t read = digitalRead(ledPin);
        digitalWrite(ledPin, !read);
        delay(1000);
    }

#elif defined(TEST_PWM)
    PulseController led;

    void setup() {
        Serial.begin(115200);
        led.setup(ledPin, false);
        // led.singlePulses(100);
        // led.doublePulses(100);
        // led.pulse20ms();
        led.uniformPulse500ms();
    }

    void loop() {
        led.run();
    }

#else
    void setup() {
        Serial.begin(115200);
    }

    void loop() {

    }
#endif
