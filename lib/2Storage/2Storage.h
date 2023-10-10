#ifdef ESP32
    #include "LITTLEFS.h"
#else
    #include "LittleFS.h"
#endif

#include "Sto_RTC.h"
#include "Sto_LittleFS.h"
#include "Sto_EEPROM.h"

#define EEPROM_SIZE 2000

struct RTC_Data {
    unsigned long bootCount = 0;
};

template <int addr1, size_t len1, int addr2, size_t len2>
class StoragePairValues {
    EEPROM_FixData<addr1, len1> value1Data;
    EEPROM_FixData<addr2, len2> value2Data;

    protected:
        char value1[len1], value2[len2];

    public:
        virtual void reloadData() {
            AppPrint("[StoPair]", __func__);
            value1Data.loadFixedData(value1);
            value2Data.loadFixedData(value2);
        }

        void storeData(const void* data1, const void* data2) {
            AppPrint("\n[StoPair]", __func__);
            value1Data.storeFixedData(data1);
            value2Data.storeFixedData(data2);

            //! reload stored data
            reloadData();
        }

        void deleteData() {
            value1Data.deleteData();
            value2Data.deleteData();
        }
};

class StorageCred: public StoragePairValues<9, 33, 43, 64> {
    public:
        const char* ssid()    { return value1; }
        const char* passw()   { return value2; }

        void reloadData() override {
            StoragePairValues::reloadData();
            AppPrint("[StoCred] SSID", ssid());
            AppPrint("[StoCred] PASSW", passw());
        }
};      

class StorageConfig: public StoragePairValues<108, 21, 130, 21> {
    public:
        const char* devName()    { return value1; }
        const char* mqttIp()     { return value2; }
};


#define MAX_BEHAVIOR_ITEMS 10
BehaviorItem APP_BEHAVIORS[MAX_BEHAVIOR_ITEMS];

class StorageBehavior {
    EEPROM_FixData<500, sizeof(APP_BEHAVIORS)> behaviorsData;

    public:
        void reloadData() {
            behaviorsData.loadFixedData(&APP_BEHAVIORS);
            AppPrint("\n[Behav] Size", sizeof(APP_BEHAVIORS));
        }

        void storeData() {
            behaviorsData.storeFixedData(&APP_BEHAVIORS);
        }

        void deleteData() {
            behaviorsData.deleteData();
        }

        void load(uint8_t index, BehaviorItem item) {
            APP_BEHAVIORS[index] = item;
        }

        void handle(Cue_Trigger cue) {
            for (int i=0; i<MAX_BEHAVIOR_ITEMS; i++) {
                (APP_BEHAVIORS[i].handle(cue));
            }
        }
};

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
        EEPROM_ResetCount resetCount;
        StorageCred stoCred;
        StorageConfig stoConfig;
        StorageBehavior stoBehavior;
        Sto_LittleFS littleFS;
        Sto_SD sd1;

        char sensorDataPath[32] = "";
        bool isValidPath()  { return String(sensorDataPath).isEmpty() == false && sd1.isReady(); }

        void setup() {
            EEPROM.begin(EEPROM_SIZE);
            resetCount.loadValue();
            
            char val[32];
            sprintf(val, "%llu", resetCount.value);
            AppPrintSeparator("[ResetCount]", val);
            stoCred.reloadData();
            stoConfig.reloadData();
            stoBehavior.reloadData();

            stoBehavior.handle(TRIGGER_SINGLECLICK);
            stoBehavior.handle(TRIGGER_DOUBLECLICK);
            AppPrint("[Sto] behaviorSize", sizeof(APP_BEHAVIORS));

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

        void testSetupData() {
            stoCred.storeData("ffffffffff22222222223333333333", "gggggggggg3333333333444444444455555555556666666666");
            stoConfig.storeData("hhhhhhhhhhjjjjjjjjjj", "kkkkkkkkkkmmmmmmmmmm");

            ControlOutput action1;
            action1.load(11, 22);
            BehaviorItem behav1;
            // behav1.configure<ControlOutput>(TRIGGER_SINGLECLICK, action1);
            // stoBehavior.load(0, behav1);

            // ControlWS2812 action2;
            // action2.load(33, 223344);
            // BehaviorItem behav2;
            // behav2.configure<ControlWS2812>(TRIGGER_DOUBLECLICK, action2);
            // stoBehavior.load(1, behav2);

            // ControlSend action3;
            // BehaviorItem behav3;
            // behav3.configure<ControlSend> (TRIGGER_SINGLECLICK, action3);
            // stoBehavior.load(2, behav3);

            // stoBehavior.storeData();
        }

        void deleteData() {
            AppPrint("[Sto]", __func__);
            resetCount.deleteValue();
            stoCred.deleteData();
            stoConfig.deleteData();
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