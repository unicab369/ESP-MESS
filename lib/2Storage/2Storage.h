#include <LittleFS.h>

#include "Sto_RTC.h"
#include "Sto_LittleFS.h"
#include "Sto_EEPROM.h"
#include "Sto_Behavior.h"

#define EEPROM_SIZE 2000

enum RESET_Type {
   RESET_WIFI,
   RESET_DEVICE,
   RESET_NONE,
};

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

//! WARNING: This object get stored in EEPROM. Keep size minimal, DO NOT inherit
class Sto_Stat: public EEPROM_Value<DeviceStats>{
   public:
      uint64_t resetCnt() { return value.resetCnt; }

      void load(uint16_t address) {
         loadData(address);
         value.increseResetCnt();
         storeData();
      }
};


// bool extractValues(const char* key, char* input, char *value1, char *value2) {
//    //# WARNING: strtok detroys the original string, perform operation on copied string
//    char inputStr[240] = "";
//    memcpy(inputStr, input, sizeof(inputStr));
//    char *ref = strtok(inputStr, " ");

//    //! check key
//    if (strcmp(ref, key) != 0) return false;
   
//    //! validate 1st value
//    ref = strtok(NULL, " ");
//    // if (ref == nullptr || strlen(ref) < 1) return false;   
//    if (ref != nullptr && strlen(ref)>0) {
//       strcpy(value1, ref);
//    }
   
//    //! validate 2nd value
//    ref = strtok(NULL, " ");
//    // if (ref == nullptr || strlen(ref) < 1 || value2 == nullptr) return false;   
//    if (ref != nullptr && strlen(ref) > 0 && value2 !=nullptr) {
//       ref[strlen(ref)] = '\0';            //! add string terminator
//       strcpy(value2, ref);
//    }

//    return true;
// }

//! WARNING: This object get stored in EEPROM. Keep size minimal, DO NOT inherit
struct Data_Cred {
   char ssid[33] = "", password[64] = "";

   void printData() {
      Loggable logger = Loggable("Data_Cred");
      Serial.println();
      logger.xLogf("ssid = %s", ssid);
      logger.xLogf("passw = %s", password);  
   }
};

class Sto_Cred: public EEPROM_Value<Data_Cred> {
   public:
      bool handleCommand(char* input) {
         if (strcmp(input, "cred") == 0) {
            value.printData();
            return true;
         }
         else if (storeData("ssid", input, value.ssid)) {
            return true;
         }
         else if (storeData("passw", input, value.password)) {
            return true;
         }

         return false;
      }
};

//! WARNING: This object get stored in EEPROM. Keep size minimal, DO NOT inherit
struct Data_Conf {
   char deviceName[21] = "", mqttIP[21] = "";

   void printData() {
      Loggable logger = Loggable("Data_Conf");
      Serial.println();
      logger.xLogf("deviceName = %s", deviceName);
      logger.xLogf("mqttIP = %s", mqttIP);  
   }
};

class Sto_Conf: public EEPROM_Value<Data_Conf> {
   public:
      bool handleCommand(char* input) {
         if (strcmp(input, "devConf") == 0) {
            value.printData();
            return true;
         }
         else if (storeData("deviceName", input, value.deviceName)) {
            return true;
         }
         else if (storeData("mqttIP", input, value.mqttIP)) {
            return true;
         }

         return false;
      }
};

struct Data_IotPlotter {
   char apiKey[63] = "", feedId[32] = "";

   void printData() {
      Loggable logger = Loggable("Data_IotPlotter");
      Serial.println();
      logger.xLogf("apiKey = %s", apiKey);
      logger.xLogf("feedId = %s", feedId);  
   }
};

class Sto_IotPlotter: public EEPROM_Value<Data_IotPlotter> {
   public:
      bool handleCommand(char* input) {
         if (strcmp(input, "iotPlotter") == 0) {
            value.printData();
            return true;
         } 
         else if (storeData("apiKey", input, value.apiKey)) {
            return true;
         }
         else if (storeData("feedId", input, value.feedId)) {
            return true;
         }
         
         return false;
      }
};

struct Data_Settings {
   bool xSerialEnable = true;
   bool espNowEnable = true;
   bool remotePlot = false;

   void printData() {
      Loggable logger = Loggable("Data_Settings");
      logger.xLogf("xSerial = %d", xSerialEnable);
      logger.xLogf("espNow = %d", espNowEnable);
      logger.xLogf("remotePlot = %d", remotePlot);
   }
};

class Sto_Settings: public EEPROM_Value<Data_Settings> {
   public:
      bool handleCommand(char* input) {
         if (strcmp(input, "settings") == 0) {
            value.printData();
            return true;
         }
         else if (storeBoolean("xSerial", input, &value.xSerialEnable)) {
            return true;
         }
         else if (storeBoolean("espNow", input, &value.espNowEnable)) {
            return true;
         }
         else if (storeBoolean("remotePlot", input, &value.remotePlot)) {
            return true;
         }

         return false;
      }
};

#define MAX_VALUE_QUEUE 15

enum DataStoreType {
   DATA_SENSOR1,
   DATA_SENSOR2,
};

class DataStoreItem {
   DataStoreType type = DATA_SENSOR1;

   public:
      char id[20] = "invalid";
      uint32_t timeStamp;
      float val1, val2, val3, val4, val5;

      String formatForStorage() {
         char output[48];
         sprintf(output, "%d %.2f %.2f %.2f %.2f %.2f %lu\n", type,
               val1, val2, val3, val4, val5, timeStamp);
         return String(output);
      }
};

class Mng_Storage: public Loggable {
   RTC_Data rtc_data;
   AppQueue<DataStoreItem, MAX_VALUE_QUEUE> dataStoreQueue;

   // void saveBootCount() {
   //     rtc_storage.write(65, &rtc_data, sizeof(rtc_data));
   // }

   // void readBootCount() {
   //     rtc_storage.read(65, &rtc_data, sizeof(rtc_data));
   // }

   public:
      Sto_RTC rtc_storage;
      Sto_Stat stoStat;             
      Sto_Cred stoCred;             
      Sto_Conf stoConf;             
      Sto_IotPlotter stoPlotter;    
      Sto_Settings stoSettings;     
      Sto_Peer stoPeer;
      Sto_Behavior stoBehavior;

      // Sto_Peer stoPeer;                      //! length 17*Count(20) [192 - 532/536]
      
      Sto_LittleFS littleFS;
      Sto_SD sd1;

      char sensorDataPath[32] = "";
      bool isValidPath()  { return String(sensorDataPath).isEmpty() == false && sd1.isReady(); }

      Mng_Storage(): Loggable("Mng_Sto") {}

      int valX = 0;

      void setupStorage() {
         xLogSection(__func__);

         EEPROM.begin(EEPROM_SIZE);
         stoStat.load(0);                 //! start 0 len 17 
         stoCred.loadData(20);            //! start 20 len 98
         stoConf.loadData(120);           //! start 120 len 43
         stoPlotter.loadData(170);        //! start 170 len 96
         stoSettings.loadData(270);       //! start 270 len 4
         stoPeer.loadData(280);           //! start 280, Count(5) * len 17
         stoBehavior.loadData(380);       //! start 380
         
         xLogSectionf("resetCount = %llu", stoStat.resetCnt());

         // stoPeer.load(192);
         // stoBehavior.reloadData();

         // littleFS.begin();
         // Serial.println("\n\n***LittleFS test");
         // littleFS.test();
      }

      //! handleConsoleCmd:
      RESET_Type handleConsoleCmd(char* inputStr) {
         Serial.println();
         if (strlen(inputStr)<1) {
            xLogf("%s %s", __func__, "invalid input");
            return RESET_NONE;
         }

         xLogf("%s %s", __func__, inputStr);
         if (strcmp(inputStr, "ping") == 0)        Serial.println("What is thy bidding my Master?");
         if (strcmp(inputStr, "deleteAll") == 0)   deleteAllData();

         //# Credential
         else if (stoCred.handleCommand(inputStr)) { }

         //# Device Configuration    
         else if (stoConf.handleCommand(inputStr)) { }

         //# IotPlotter
         else if (stoPlotter.handleCommand(inputStr)) { }

         //# Settings
         else if (stoSettings.handleCommand(inputStr)) { }

         //# peers
         else if (stoPeer.handleCommand(inputStr)) { }

         //#
         else if (strcmp(inputStr, "getSens") == 0) {
            Serial.println(valX++);
         }

         return RESET_NONE;
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

      void storeItem(DataStoreItem *item) {
         char output[32];
         sprintf(output, "%.2f %.2f %.2f %.2f %.2f %ld\n", item->val1, item->val2, 
               item->val3, item->val4, item->val5, (long)item->timeStamp);
         dataStoreQueue.sendQueue(item);
      }

      void handleValueQueue(std::function<void(uint32_t)> onComplete) {
         DataStoreItem item;
         if (!dataStoreQueue.getQueue(&item)) return;
         // AppPrint("StoreValue", item.value);

         uint32_t timeRef = millis();
         // String output = item.formatForStorage();
         // Serial.println("Storing data");
         // Serial.println(output);
         // sd1.appendFile(sensorDataPath, output.c_str());
         onComplete(millis()-timeRef);
      }

      void deleteAllData() {
         AppPrint("[Sto]", __func__);
         stoStat.deleteData();
         stoCred.deleteData();
         stoConf.deleteData();
         stoPlotter.deleteData();
         stoPeer.deleteData();
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