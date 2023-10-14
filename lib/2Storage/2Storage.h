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
   uint64_t resetCnt = 0;
   uint8_t status = 0;
};

class Sto_Stat: public EEPROM_Value<DeviceStats> {

};

struct WiFiCred {
   char ssid[33] = "";
   char password[64] = "";

   void loadValues(const char* ssidVal, const char* passwVal) {
      memcpy(ssid, ssidVal, sizeof(ssid));
      memcpy(password, passwVal, sizeof(password));
   }
};

class Sto_Cred: public EEPROM_Value<WiFiCred> {
   public:
      void reloadData() {
         loadData(9);
         AppPrint("\n[StoCred] SSID", data.ssid);
         AppPrint("\n[StoCred] PASSW", data.password);         
      }

      void updateData(const char* ssidVal, const char* passwVal) {
         data.loadValues(ssidVal, passwVal);
         storeData();
      }
}; 

// // template format: startAddr of devName, devName length, startAddr of mqttIp, mqttIp length
// class Sto_Config: public Sto_PairValues<108, 21, 130, 21> {
//    public:
//       const char* devName()    { return value1; }
//       const char* mqttIp()     { return value2; }
// };



#define MAX_VALUE_QUEUE 10

struct StoringValue {
   char value[32];
};

class Mng_Storage {
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
      // EEPROM_ResetCount resetCount;
      Sto_Cred stoCred;
      // Sto_Config stoConfig;
      Sto_Behavior stoBehavior;
      Sto_LittleFS littleFS;
      Sto_SD sd1;

      char sensorDataPath[32] = "";
      bool isValidPath()  { return String(sensorDataPath).isEmpty() == false && sd1.isReady(); }

      void setup() {
         EEPROM.begin(EEPROM_SIZE);
         // resetCount.loadValue();
         
         // char val[32];
         // sprintf(val, "%llu", resetCount.value);
         // AppPrintSeparator("[ResetCount]", val);
         stoCred.reloadData();
         // stoConfig.reloadData();
         // stoBehavior.reloadData();

         // littleFS.begin();
         // Serial.println("\n\n***LittleFS test");
         // littleFS.test();
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
         // resetCount.deleteValue();
         stoCred.deleteData();
         // stoConfig.deleteData();
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