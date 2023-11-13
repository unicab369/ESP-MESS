
// #define TTGO_LORA32 true
// #define DEV_KMC_70011 1
#define ESP_MESS_V3 1

class Mng_Config: public Serv_IO {
    public:
        void setup() {
            #ifdef TTGO_LORA32
                // xLogSection("DIRECTIVE = TTGO_LORA32");
                // pinMode(16, OUTPUT);
                // digitalWrite(16, HIGH);
                // delay(20);

                // pinMode(17, INPUT_PULLUP);
                // pinMode(2, OUTPUT);
                // sda0 = 4; scl0 = 15;
                miso1 = 19; mosi1 = 23; sck1 = 18;  cs1 = 5;
                di01 = 2; rst1 = 14;

                vspi->begin(sck1, miso1, mosi1, cs1);
                SPI.begin(sck1, miso1, mosi1, cs1);
                // lora.setup(cs1, rst1, di01, *vspi);
                
                // lora.loadReceiveCb([this](Lora_Packet *packet) {
                //     Serial.println("LOra received"); 
                //     packet->print();
                //     // i2c.disp.printline("Lora: " + String(packet->data0), 5);
                //     // digitalWrite(2, !digitalRead(2));
                // });

            #elif defined(DEV_KMC_70011)
                xLogSection("DIRECTIVE = DEV_KMC_70011");
                led1 = 13;
                btn1 = 0;
                relay1 = 14;

            #elif CONFIG_IDF_TARGET_ESP32C3
                Serial.println("IM HERE zzz 333");
            
            #elif ESP_MESS_V3
                led1 = 22;
                btn1 = 16;
                rotaryA = 13; rotaryB = 15;
                
                //! i2C Pins
                sda0 = 33; scl0 = 32;
                sda1 = 0; scl1 = 2;

                //! xSerial
                swRx = 25; swTx = 26;

            #elif ESP32
                //! Lolin missing 1TX, 3RX, 21, 24
                // xLogSection("DIRECTIVE = ESP32");
                // use for disp0
                // pinMode(5, OUTPUT);
                // pinMode(18, OUTPUT);
                // digitalWrite(5, LOW);
                // digitalWrite(18, HIGH);

                // ws2812 = 12;
                // led1 = 22;
                // sda0 = 32; scl0 = 33;
                // pir1 = 36;
                // irSwitch = 26;
                // btn1 = 23;
                // buzzer1 = 19;

                // ws2812p = 12;
                led1 = 22;
                btn1 = 34;
                // buzzer1 = 14;
                // rotaryA = 13; rotaryB = 15;
                // pir1 = 36;
                // irSwitchp = 15;  //! Need update
                // swRx = 0; swTx = 2;
                
                //! i2C Pins
                sda0 = 33; scl0 = 32;
                sda1 = 4; scl1 = 0;

                //! SPI Pins
                sck0 = 18; mosi0 = 23; miso0 = 19; rst0 = 2;
                
                //! large display: CS low enable
                
                out0 = 5;        // sdCard CS
                out1 = 27;       // display CS
                out2 = 26;       // display AO
                out3 = 25;       // display backlight

            #else
                // xLogSection("DIRECTIVE = ESP8266");
                
                // D0: 16, D1: 5, D2: 4, D3: 0, D4: 2, D5: 14, D6: 12, D7: 13, D8: 15
                sda0 = 5; scl0 = 4;
                led1 = 2;
                btn1 = 0;
                // relay1 = 14;
                // ws2812 = 15;
                // irSwitch = 12;
                // pir1 = 14;

                // SPI.pins(14, 12, 13, 15);
                // SPI.begin();
                // swRx = 14; swTx = 12;
            #endif

            setupIOs();        
        }
};