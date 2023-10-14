#ifdef ESP32
   #include "LITTLEFS.h"
#else
   #include "LittleFS.h"
#endif

#include "Sto_RTC.h"
#include "Sto_LittleFS.h"
#include "Sto_EEPROM.h"
#include "Sto_Behavior.h"

#define EEPROM_SIZE 2000

struct RTC_Data {
   unsigned long bootCount = 0;
};

struct DeviceStats {
   uint8_t status = 0;
   uint16_t builtCode = 0;
   uint64_t resetCnt = 0;

   void increseResetCnt() {
      resetCnt++;
   }
};

template <uint16_t address>
class Sto_Stat: public EEPROM_Value<DeviceStats>, public Loggable {
   public:
      Sto_Stat(): Loggable("Sto_Stat") {}

      void reloadData() {
         loadData(address);
         value.increseResetCnt();
         storeData();
         xLogSectionf("resetCount = %llu", value.resetCnt);
      }
};

template <uint8_t len1, uint8_t len2>
class PairChar {
   public:
      char value1[len1];
      char value2[len2];

      bool getValues(const char* key, char* input) {
         //! strtok detroys the original string, copy it before perform operation
         char inputStr[124];
         memcpy(inputStr, input, sizeof(inputStr));
         char *ref = strtok(inputStr, " ");

         if (strcmp(ref, key) == 0) {
            ref = strtok(NULL, " ");
            strcpy(value1, ref);

            ref = strtok(NULL, " ");
            ref[strlen(ref) - 1] = '\0';  // Replace '\n' with string terminator
            strcpy(value2, ref);

            return true;
         }

         return false;
      }

      void loadVals(PairChar* newVal) {
         strcpy(value1, newVal->value1);
         strcpy(value2, newVal->value2);
      }
};

class WiFiCred: public PairChar<33, 64> {
   public:
      const char* ssid() { return value1; }
      const char* password() { return value2; }
};

class DevConf: public PairChar<21,21> {
   public:
      const char* name() { return value1; }
      const char* mqttIP() { return value2; }
};

template <uint16_t address>
class Sto_Cred: public EEPROM_Value<WiFiCred>, public Loggable {
   public:
      Sto_Cred(): Loggable("Sto_Cred") {}

      void reloadData() {
         loadData(address);
         xLogf("SSID = %s", value.ssid());
         xLogf("PASSW = %s", value.password());      
      }

      void updateData(WiFiCred* newVal) {
         value.loadVals(newVal);
         storeData();
         reloadData();
      }
};

template <uint16_t address>
class Sto_Config: public EEPROM_Value<DevConf>, public Loggable {
   public:
      Sto_Config(): Loggable("Sto_Config") {}

      void reloadData() {
         loadData(address);
         xLogf("name = %s", value.name());
         xLogf("mqttIP = %s", value.mqttIP());    
      }

      void updateData(DevConf* newVal) {
         value.loadVals(newVal);
         storeData();
         reloadData();
      }
};

#define MAX_VALUE_QUEUE 10

struct StoringValue {
   char value[32];
};

class Mng_Storage: public Loggable {
   RTC_Data rtc_data;
   AppQueue<StoringValue, MAX_VALUE_QUEUE> valueQueue;

   // void saveBootCount() {
   //     rtc_storage.write(65, &rtc_data, sizeof(rtc_data));
   // }

   // void readBootCount() {
   //     rtc_storage.read(65, &rtc_data, sizeof(rtc_data));
   // }

   public:
      Sto_RTC rtc_storage;
      Sto_Stat<0> stoStat;    //! length 17 [end 17]
      Sto_Cred<50> stoCred;   //! length 98 [end 130]
      Sto_Config<200> stoConf;

      // Sto_Config stoConfig;
      Sto_Behavior stoBehavior;
      Sto_LittleFS littleFS;
      Sto_SD sd1;

      char sensorDataPath[32] = "";
      bool isValidPath()  { return String(sensorDataPath).isEmpty() == false && sd1.isReady(); }

      Mng_Storage(): Loggable("Mng_Sto") {}

      void setup() {
         EEPROM.begin(EEPROM_SIZE);
         stoStat.reloadData();
         stoCred.reloadData();
         stoConf.reloadData();
         // stoBehavior.reloadData();

         // littleFS.begin();
         // Serial.println("\n\n***LittleFS test");
         // littleFS.test();
      }

      void handleConsoleStr(char* inputStr) {
         WiFiCred cred;
         DevConf conf;

         if (cred.getValues("cred", inputStr)) {
            Serial.println();
            xLogf("ssid = %s, passw = %s", cred.ssid(), cred.password());
            stoCred.updateData(&cred);
         }
         else if (conf.getValues("conf", inputStr)) {
            Serial.println();
            xLogf("name = %s, mqtt = %s", conf.name(), conf.mqttIP());
            stoConf.updateData(&conf); 
         }
      }

      void setupSDCard(uint8_t sdCS) {
         if (sdCS == 255) return;
         sd1.begin(sdCS);
         // sd1.test();
      }

      void loadStoragePath(String dateTimeStr) {
         if (dateTimeStr.isEmpty() || !sd1.isReady()) {
               AppPrint("[Sto] Err: sd loadPath Failed");
               return;
         }
         String path = "/sensors/" + dateTimeStr + "/reading.txt";
         memcpy(sensorDataPath, path.c_str(), sizeof(sensorDataPath));
         sd1.makeFile(sensorDataPath);
      }

      void addStoreTempHumLuxQueue(float temp, float hum, float lux, time_t timeStamp) {
         char output[32];
         sprintf(output, "%.2f %.2f %.2f %ld\n", temp, hum, lux, (long)timeStamp);

         StoringValue storeValue;
         strcpy(storeValue.value, output);
         valueQueue.sendQueue(&storeValue);
      }

      void handleValueQueue(std::function<void(uint32_t)> onComplete) {
         StoringValue item;
         if (!valueQueue.getQueue(&item)) return;
         // AppPrint("StoreValue", item.value);

         uint32_t timeRef = millis();
         sd1.appendFile(sensorDataPath, item.value);
         onComplete(millis()-timeRef);
      }

      void deleteData() {
         AppPrint("[Sto]", __func__);
         stoStat.deleteData();
         stoCred.deleteData();
         stoConf.deleteData();
         stoBehavior.deleteData();
      }

      // void resetBootCnt() {
      //     // bootCount = 0;
      //     rtc_data.bootCount = 0;
      //     saveBootCount();
      // }

      // void incBootCnt() {
      //     // bootCount++;
      //     rtc_data.bootCount++;
      //     saveBootCount();
      // }

      // unsigned long getBootCnt() {
      //     readBootCount();
      //     #ifndef ESP32
      //         return rtc_data.bootCount;
      //     #else
      //         // return bootCount;
      //     #endif
      // }
};