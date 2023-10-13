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
    Sto_Behavior stoBehav;

    void setup() {
        Serial.begin(115200);
        storage.setup();
        stoBehav.setup();

        BehaviorItem behav_In;
        BehaviorItem* behav_Out;

        ControlOutput action1(11, 22);
        behav_In.store(0, TRIGGER_SINGLECLICK, action1);
        stoBehav.updateData(0, &behav_In);                  //! store behavior
        stoBehav.handleCue(0, TRIGGER_SINGLECLICK);

        ControlWS2812 action2(33, 44);
        behav_In.store(0, TRIGGER_DOUBLECLICK, action2);
        stoBehav.updateData(1, &behav_In);                  //! store behavior
        stoBehav.handleCue(0, TRIGGER_DOUBLECLICK);
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
