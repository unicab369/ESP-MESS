#include <TinyGPSPlus.h>


class Mod_GPS: private Loggable {
    void displayInfo() {
        // Serial.print(F("Location: ")); 
        if (gps.location.isValid()) {
            Serial.print(gps.location.lat(), 6);
            Serial.print(F(","));
            Serial.print(gps.location.lng(), 6);
        } else {
            Serial.print(F("INVALID"));
        }

        // Serial.print(F("  Date/Time: "));
        // if (gps.date.isValid()) {
        //     Serial.print(gps.date.month());
        //     Serial.print(F("/"));
        //     Serial.print(gps.date.day());
        //     Serial.print(F("/"));
        //     Serial.print(gps.date.year());
        // } else {
        //     Serial.print(F("INVALID"));
        // }

        // Serial.print(F(" "));
        // if (gps.time.isValid()) {
        //     if (gps.time.hour() < 10) Serial.print(F("0"));
        //     Serial.print(gps.time.hour());
        //     Serial.print(F(":"));
        //     if (gps.time.minute() < 10) Serial.print(F("0"));
        //     Serial.print(gps.time.minute());
        //     Serial.print(F(":"));
        //     if (gps.time.second() < 10) Serial.print(F("0"));
        //     Serial.print(gps.time.second());
        //     Serial.print(F("."));
        //     if (gps.time.centisecond() < 10) Serial.print(F("0"));
        //     Serial.print(gps.time.centisecond());
        // } else {
        //     Serial.print(F("INVALID"));
        // }

        Serial.println();
    }

    public:
        TinyGPSPlus gps;

        Mod_GPS(): Loggable("GPS") {}

        bool handleData(char read) {
            if (gps.encode(read)) {
                displayInfo();
                return true;
            }

            return false;
        }

        // void run() {
        //     if (gps.encode(ss.read())) displayInfo();
        // }
};

#ifdef ESP32
    class ExtraSerial {
        Mod_GPS gps;
        bool didLoad = false;

        public:
            void setup(int8_t rx, int8_t tx) {
                if (rx == 255 || tx == 255) return;
                Serial1.begin(9600, SERIAL_8N1, rx, tx);
                didLoad = true;
            }

            void run() {
                if (!didLoad || !Serial1.available()) return;
                gps.handleData(Serial1.read());
                // if (gps.handleData(Serial1.read())) {
                    // String receivedData = Serial1.readStringUntil('\n'); // Read the incoming message
                    // Serial.println("Received: " + receivedData); // Print the received message
                // }
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