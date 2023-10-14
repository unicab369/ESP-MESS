#ifdef ESP32 
    uint8_t ledPin = 22;
#else
    uint8_t ledPin = 2;
#endif

// #define TEST_PCA96
#define TEST_BEHAVIOR True
// #define TEST_PWM True

Loggable TestLog("Test"); 

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

#elif defined(TEST_PCA96)
    PCA96Controller pca96z;

    void setup() {
        Serial.begin(115200);
        Wire.begin(33, 32);

        pca96z.setup(&Wire);
        delay(10);
    }

    void loop() {
        pca96z.test();
    }

#elif defined(TEST_BEHAVIOR)
    Mng_Storage storage;
    Serv_Behavior servBehav;
    
    void setup() {
        Serial.begin(115200);
        storage.setup();
        servBehav.setup();
        // servBehav.deleteData();

        uint8_t peer1Mac[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x88};
        ControlOutput action1(11, 22);    
        servBehav.storeAction<TRIGGER_SINGLECLICK>(0, &action1, peer1Mac);

        // ControlWS2812 action2(33, 44);
        // servBehav.storeAction<TRIGGER_DOUBLECLICK>(1, &action2, &peer1);

        servBehav.test();
    }

    void loop() {

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
