#ifdef ESP32 
    uint8_t ledPin = 22;
#else
    uint8_t ledPin = 2;
#endif


#ifndef TEST_BLINK
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
#else
    void setup() {
        Serial.begin(115200);
    }

    void loop() {

    }
#endif
