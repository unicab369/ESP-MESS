#include <TinyGPSPlus.h>

class DataCollector {
    protected:
        uint8_t buff[32] = { 0 };
        uint8_t idx = 0;

        void resetBuffer() {
            idx=0;
            memset(buff, 0, sizeof(buff));
        }

        void printReceivedData(const uint8_t *data, uint8_t len) {
            Serial.println();
            for (int i=0; i<idx; i++) Serial.printf("%02X ", data[i]);
            Serial.println();
            if (callback!=nullptr) (*callback)(data);
        }

    public:
        std::function<void(const uint8_t*)> *callback;

        void addData(uint8_t value) {
            buff[idx++] = value;
            if (idx>sizeof(buff)) resetBuffer();
        }

        void completeData() {
            printReceivedData(buff, sizeof(buff));
            resetBuffer();
        }
};

class DataInterpreter: public DataCollector {
    public:
        void makeData() {
            uint16_t key = (buff[1]<<8) | buff[0];
            uint8_t addr[4] = { buff[2], buff[3], buff[4], buff[5] };
            uint16_t val1 = (buff[7]<<8) | buff[6];
            uint16_t val2 = (buff[9]<<8) | buff[8];
            uint16_t val3 = (buff[11]<<8) | buff[10];
            uint16_t val4 = (buff[13]<<8) | buff[12];

            Serial.printf("\ntemp=%lu | hum=%lu | lux=%lu | volt=%lu", val1, val2, val3, val4);
            completeData();
        }
};

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
    class ExtraSerial: private Loggable {
        Mod_GPS gps;
        bool didLoad = false;

        public:
            DataInterpreter interpreter;
            std::function<void(const uint8_t*)> *callback;
            ExtraSerial(): Loggable("xSerial") {}

            void setup(int8_t rx, int8_t tx) {
                if (rx == 255 || tx == 255) return;
                Serial1.begin(9600, SERIAL_8N1, rx, tx);
                didLoad = true;
            }

            void run() {
                if (!didLoad || !Serial1.available()) return;
                xLog(__func__);

                while(Serial1.available()>0) {
                    uint8_t read = Serial1.read();

                    //! check for 0x0D (Carriage Return)
                    if (read == 0x0D) {
                        //! check for 0x0A (Line Feed)
                        if (Serial1.peek() == 0x0A) {
                            Serial1.read();  // read and store Carriage Return
                            interpreter.makeData();
                        }
                    } else {
                        interpreter.addData(read);
                    }
                }
                // gps.handleData(Serial1.read());
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