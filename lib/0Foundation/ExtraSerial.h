#ifdef ESP32
    class ExtraSerial {
        bool didLoad = false;

        public:
            void setup(int8_t rx, int8_t tx) {
                if (rx == 255 || tx == 255) return;
                Serial1.begin(9600, SERIAL_8N1, tx, rx);
                didLoad = true;
            }

            void run() {
                if (!didLoad || !Serial1.available()) return;
                String receivedData = Serial1.readStringUntil('\n'); // Read the incoming message
                Serial.println("Received: " + receivedData); // Print the received message
            }

            void println(const char *msg) {
                if (!didLoad) return;
                Serial1.println(msg);
            }
    };

#else
    #include <SoftwareSerial.h>

    class ExtraSerial {
        SoftwareSerial swSerial;
        bool didLoad = false;

        public:
            void setup(int8_t rx, int8_t tx) {
                if (rx == 255 || tx == 255) return;
                swSerial.begin(9600, SWSERIAL_8N1, rx, tx);
                didLoad = true;
            }

            void run() {
                if (!didLoad || !swSerial.available()) return;
                String receivedData = swSerial.readStringUntil('\n'); // Read the incoming message
                Serial.println("Received: " + receivedData); // Print the received message
            }

            void println(const char *msg) {
                if (!didLoad) return;
                swSerial.println(msg);
            }
    };
#endif