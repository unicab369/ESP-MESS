// #include <RH_RF69.h>

// #define SCK 18
// #define MISO 19
// #define MOSI 23
// #define SS 5
// #define RST 14
// #define DIO0 2
// #define BAND 915

// #define RFM_RECEIVER true

// RH_RF69 rf69;

class Net_Radio {
    public:
        void setup(uint8_t ss, uint8_t dio) {  
            // SPI.begin(SCK, MISO, MOSI, SS);

            // String status = rf69.init(ss, dio) ? "Successfull" : "Failed";
            // Serial.println("\n[RF69] Init " + status);
            // if (!rf69.setFrequency(915.0)) Serial.println("setFrequency failed");
            // // rf69.setModemConfig(RH_RF69::GFSK_Rb57_6Fd120);
            // // High power RF69 eg RFM69HW, *must* set a Tx power with the ishighpowermodule flag:
            // rf69.setTxPower(14, true);

            // // The encryption key has to be the same as the one in the client
            // uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
            //                   0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
            // rf69.setEncryptionKey(key);
            
            #if 0
                // For compat with RFM69 Struct_send
                rf69.setModemConfig(RH_RF69::GFSK_Rb250Fd250);
                rf69.setPreambleLength(3);
                uint8_t syncwords[] = { 0x2d, 0x64 };
                rf69.setSyncWords(syncwords, sizeof(syncwords));
                rf69.setEncryptionKey((uint8_t*)"thisIsEncryptKey");
            #endif
        }

        void sendPacket(Lora_Packet* packet) {
            // rf69.send((uint8_t*)packet, sizeof(Lora_Packet));
            // rf69.waitPacketSent();
        }

        // void run() {
        //     #ifdef RFM_RECEIVER
        //         if (!rf69.available()) { return; }

        //         Lora_Packet packet;
        //         uint8_t len = sizeof(Lora_Packet);
        //         rf69.recv((uint8_t*)&packet, &len);

        //         Serial.print("\nRSSI: "); Serial.println(rf69.lastRssi(), DEC);
        //         packet.print();
        //     #else
        //         uint8_t source[] = "1111";
        //         char data[] = "Hello";
        //         Lora_Packet packet = Lora_Packet::make(source, data);
        //         sendPacket(&packet);
        //         delay(2000);
        //         Serial.println("Send Lora");
        //     #endif
        // }

        void run() {
            #ifdef RFM_RECEIVER
                if (rf69.available()) {
                // Should be a message for us now   
                uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
                uint8_t len = sizeof(buf);
                if (rf69.recv(buf, &len)) {
            //      RH_RF69::printBuffer("request: ", buf, len);
                    Serial.print("got request: "); Serial.println((char*)buf);
                    Serial.print("RSSI: "); Serial.println(rf69.lastRssi(), DEC);
                    
                    // Send a reply
                    uint8_t data[] = "And hello back to you";
                    rf69.send(data, sizeof(data));
                    rf69.waitPacketSent();
                    Serial.println("Sent a reply");

                } else {
                    Serial.println("recv failed");
                }
                }
            #else
                Serial.println("Sending to rf69_server");
                // Send a message to rf69_server
                // uint8_t data[] = "Hello World!";
                // rf69.send(data, sizeof(data));
                // rf69.waitPacketSent();

                // // Now wait for a reply
                // uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
                // uint8_t len = sizeof(buf);

                // if (rf69.waitAvailableTimeout(500)) { 
                //     // Should be a reply message for us now   
                //     if (rf69.recv(buf, &len)) {
                //         Serial.print("got reply: ");
                //         Serial.println((char*)buf);
                //     } else {
                //         Serial.println("recv failed");
                //     }
                // }

                delay(2000);
            #endif
        }

};