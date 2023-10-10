#include <LoRa.h>

const long LORA_BAND = 915E6;  // LoRa Frequency

struct Lora_Packet {
    uint32_t timeStamp = 0;
    uint32_t code = 0;
    uint8_t source[6] = { 0 };
    char data1[50];

    void print() {
        Serial.print("timeStamp: "); Serial.println(timeStamp);
    }
    static Lora_Packet make(const uint8_t *_source, char *data, uint32_t code = 123) {
        Lora_Packet output;
        output.code = code;
        output.timeStamp = millis();
        memcpy(&output.source, _source, 6);
        memcpy(&output.data1, data, sizeof(data1));
        return output;
    }

    uint32_t getTimeDif() {
        return millis()-timeStamp;
    }
};

#ifdef ESP32
    static QueueHandle_t loraMsgQueue; 
    static const uint8_t loraMsgQueue_len = 5;
#endif

class Net_Lora {
    bool isLoaded = false;
    int sendCounter = 0;
    std::function<void(Lora_Packet*)> callback;
    uint8_t source[6] = { 0 };

    void load() {
        LoRa.setTxPower(20);
        LoRa.setSpreadingFactor(7);
        LoRa.setSignalBandwidth(125E3);

        if (!LoRa.begin(LORA_BAND)) {
            Serial.println("[LoRa] Failed");
            return;
        } else {
            Serial.println("[LoRa] Started");
            isLoaded = true;
        }

        LoRa.onReceive([](int packetSize) {
            Lora_Packet packet;
            LoRa.readBytes((uint8_t*)&packet, sizeof(Lora_Packet));
            // if (packet.code != 123) { return; }

            Serial.print("\nLoraPacket (size): "); Serial.println(packetSize);
            Serial.println(packet.code);
            Serial.println(packet.timeStamp);

            #ifdef ESP32
                if (xQueueSend(loraMsgQueue, &packet, 10) != pdTRUE) {
                    // Serial.println("[LoRa] Queue Full");
                }
            #endif
        });

        #ifdef ESP32
            loraMsgQueue = xQueueCreate(loraMsgQueue_len, sizeof(Lora_Packet));
        #endif

        LoRa.receive();                   // put the radio into receive mode            
    }

    public:
        // PostCounter postCounter;
        Net_Lora() {}

        void setup(uint8_t cs, uint8_t reset, uint8_t dio, SPIClass &spi) {
            if (cs == 255 || reset == 255 || dio == 255) { return; }
            // LoRa.setSPI(spi);
            LoRa.setPins(cs, reset, dio);
            load();
        }

        void loadReceiveCb(std::function<void(Lora_Packet*)> cb) {
            callback = cb;
            // memcpy(&source, _source, 6);
        }

        int *getCounter() { return &sendCounter; }

        void sendData(char *data) {
            if (!isLoaded) { return; }
            Lora_Packet packet = Lora_Packet::make(source, data);
            // packet.data0 = sendCounter++;
            sendPacket(&packet);
        }

        void sendPacket(Lora_Packet *packet) {
            if (!isLoaded) { return; }
            LoRa.beginPacket();
            LoRa.write((uint8_t*)packet, sizeof(Lora_Packet));
            LoRa.endPacket();
            LoRa.receive();
            // postCounter.makeSendCount();
        }

        void run() {
            if (!isLoaded || callback == NULL) { return; }
            Lora_Packet packet;

            #ifdef ESP32
                if (xQueueReceive(loraMsgQueue, (Lora_Packet*)&packet, 0) == pdTRUE) {
                    callback(&packet);
                }
            #endif
        }
};