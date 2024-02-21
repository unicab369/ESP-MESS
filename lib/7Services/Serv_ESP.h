class ESP_Device {
    public:
        char boardType[10];
        
        ESP_Device() {
            size_t size = sizeof(boardType);
            
            #if defined(ESP8266) 
                memcpy(boardType, "ESP8266", size);
            #elif defined(CONFIG_IDF_TARGET_ESP32S2)
                memcpy(boardType, "ESP32-S2", size);
            #elif defined(CONFIG_IDF_TARGET_ESP32S3)
                memcpy(boardType, "ESP32-S3", size);
            #elif defined(CONFIG_IDF_TARGET_ESP32C3)
                memcpy(boardType, "ESP32-C3", size);
            #elif defined(CONFIG_IDF_TARGET_ESP32)
                memcpy(boardType, "ESP32", size);
            #else
                memcpy(boardType, "Unknown", size);
            #endif
        }

        #ifdef ESP32 
            void restart() { ESP.restart(); }
            uint64_t chipId() { return (uint64_t)ESP.getEfuseMac(); }
            uint32_t maxAllocatedHeap()     { return ESP.getMaxAllocHeap(); }
            uint32_t heapSize()             { return ESP.getHeapSize(); }
            String getResetReason() { return String(esp_reset_reason()); }
            uint32_t randomNumber() { return esp_random(); }
            
            void fillRandomNumber(byte *buf, byte len) { 
                esp_fill_random(buf, len); 
            }
        #else
            void restart() { ESP.reset(); }
            uint32_t chipId() { return ESP.getChipId(); }
            uint32_t maxAllocatedHeap()      { return ESP.getMaxFreeBlockSize(); }
            String getResetReason() { return ESP.getResetReason(); }
            uint32_t randomNumber() { return RANDOM_REG32; }

            void fillRandomNumber(byte *buf, byte len) { 
                for (byte i=0; i<len; i++) {
                    buf[i] = randomNumber();
                }
            }
        #endif

        String chipIdHex()      {
            uint64_t id = chipId();
            uint64_t long0 = ((id & 0xFFFF00000000) >> 32);
            uint64_t long1 = ((id & 0xFFFF0000) >> 16 );
            uint64_t long2 = ((id & 0x0000FFFF));
            // String str0 = String(long0, HEX);
            // String str1 = String(long1, HEX);
            // String str2 = String(long2, HEX);
            // str0.toUpperCase();
            // str1.toUpperCase();
            // str2.toUpperCase();
            // return str0 + str1 + str2;
            return "aaa";
        }

        uint32_t flashSize()    { return ESP.getFlashChipSize()/1024; }
        uint32_t cpuFreq()      { return ESP.getCpuFreqMHz(); }
        uint32_t sketchUsed()   { return (ESP.getSketchSize())/1024; }
        uint32_t sketchTotal()  { return (ESP.getSketchSize()+ESP.getFreeSketchSpace())/1024; }

        void printSPIPins() {
            AppPrint("[SPI]", "MOSI=" + String(MOSI) + 
                            ", MISO=" + String(MISO) + 
                            ", SCK=" + String(SCK));
        }

        void printI2CPins() {
            Serial.print("SDA: "); Serial.print(SDA); 
            Serial.print(" SCL: "); Serial.println(SCL); 
        }
};

ESP_Device MY_ESP;