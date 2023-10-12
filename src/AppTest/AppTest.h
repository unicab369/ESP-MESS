#ifdef ESP32 
    uint8_t ledPin = 22;
#else
    uint8_t ledPin = 2;
#endif

#define TEST_PCA96
// #define TEST_BEHAVIOR True
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
    StorageBehavior stoBehav;

    void setup() {
        Serial.begin(115200);
        storage.setup();
        stoBehav.setup();

        ControlOutput action1;
        action1.load(11, 22);
        BehaviorItem behav1;
        behav1.configure<ControlOutput>(TRIGGER_SINGLECLICK, action1);
        stoBehav.updateData(&behav1, 0);
        
        BehaviorItem *behav1_test = stoBehav.getData(0);
        TestLog.xLogf("CHECK1 cue = 0x%02X; cmd = 0x%02X", behav1_test->cue, behav1_test->actionCmd);

        ControlWS2812 action2;
        action2.load(33, 223344);
        BehaviorItem behav2;
        behav2.configure<ControlWS2812>(TRIGGER_DOUBLECLICK, action2);
        stoBehav.updateData(&behav2, 1);

        BehaviorItem *behav2_test = stoBehav.getData(1);
        TestLog.xLogf("CHECK2 cue = 0x%02X; cmd = 0x%02X", behav2_test->cue, behav2_test->actionCmd);

        ControlSend action3;
        BehaviorItem behav3;
        behav3.configure<ControlSend> (TRIGGER_SINGLECLICK, action3);
        stoBehav.updateData(&behav3, 2);

        BehaviorItem *behav3_test = stoBehav.getData(2);
        TestLog.xLogf("CHECK3 cue = 0x%02X; cmd = 0x%02X", behav3_test->cue, behav3_test->actionCmd);
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
