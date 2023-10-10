class I2CInterface {
    TwoWire *wire;
    uint8_t address;

    public:
        void begin(TwoWire *_wire, uint8_t _addr) {
            wire = _wire;
            address = _addr;
        }
        
        bool checkDevice() {
            // uint32_t time = millis();
            wire->beginTransmission(address);
            uint8_t error = wire->endTransmission();
            // uint32_t timeDif = millis() - time;
            return error == 0;
        }

};

#include "Mod_RTC.h"
#include "SensorBase.h"
#include "Mod_SHT.h"
#include "Mod_BH17.h"
#include "Mod_VL53L0X.h"
#include "Mod_MPU6050.h"

class Serv_Sensor {
    SensorBase *sensors[2];
    bool readFree = true;
    int currentIndex = 0;

    void startNextReading() {
        currentIndex++;
        readFree = true;        // free the reading
    }
    public:
        Mod2_SHT3 sht;
        Mod2_BH17 bh17;
        bool isLoaded = false;

        void setup(TwoWire *wire) {
            sensors[0] = &sht;
            sensors[1] = &bh17;
            for (int i=0; i<2; i++) { sensors[i]->setup(wire); }
            isLoaded = true;
        }

        void reset() {
            if (!isLoaded) return; 
            for (int i=0; i<2; i++) { sensors[i]->reset(); }
            currentIndex = 0;
            readFree = true;
        }

        // return active status
        bool checkStatus() {
            if (!isLoaded || currentIndex >= 2) { return false; }
            
            if (readFree) {
                // request reading when read is free
                readFree = false;
                bool validate = sensors[currentIndex]->requestReadings();
                if (!validate) startNextReading();
            } else {
                // otherwise collect the data
                bool validate = sensors[currentIndex]->collectReadings();
                if (validate) startNextReading();
            }

            return true;
        }
};

//std::array<2, std::shared_ptr<SensorBase>> sensors
class Mng_Sensor {
    Serv_Sensor sensors;
    Mod_VL53LOX lox;
    // Mod_MPU6050 mpu;
    bool isLoaded = false;
    char output[32];

    public:
        void setup(TwoWire *wire) {
            sensors.setup(wire);
            // lox.setup();
            // mpu.setup();
            isLoaded = true;
        }

        void reset() {
            sensors.reset();
        }

        void getTempHumLux(float *temp, float *hum, float *lux) {
            *temp = sensors.sht.getTemp();
            *hum = sensors.sht.getHum();
            *lux = sensors.bh17.getLux();
        }

        char* getTempHumLux(bool markup = true) {
            if (!isLoaded) return "<null>";
            sprintf(output, markup ? "T%.2f H%.2f L%.2f" : "%.2f %.2f %.2f", 
                    sensors.sht.getTemp(), sensors.sht.getHum(), sensors.bh17.getLux());
            return output;
        }
        
        bool checkStatus() {
            if (!isLoaded) return false;
            return sensors.checkStatus();

            // lox.run();
            // mpu.run();
        }
};
