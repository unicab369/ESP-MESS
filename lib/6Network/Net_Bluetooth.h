#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "BLEEddystoneURL.h"
#include "BLEEddystoneTLM.h"
#include "BLEBeacon.h"

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

#define TARGET_DEVICE_ADDRESS "98:89:13:0a:69:9b"

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
   void onResult(BLEAdvertisedDevice advertisedDevice) {
      // Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      BLEAddress addr = advertisedDevice.getAddress();
      BLEAddress compareAddr = BLEAddress("98:89:13:0a:4e:36");
      if (!addr.equals(compareAddr)) return;

      uint8_t* payload = advertisedDevice.getPayload();
      Serial.print("addr = "); Serial.println(advertisedDevice.getAddress().toString().c_str());
      Serial.print("Payload = "); Serial.println(payload[0]);
   }
};

bool bleConnected = false;

class MyBLEClientCallbacks : public BLEClientCallbacks {
public:
   void onConnect(BLEClient* pClient) {
      // Handle connection event
      Serial.println("Connected to BLE server");
      bleConnected = true;
      Serial.println("IM HERE 2");
   }

   void onDisconnect(BLEClient* pClient) {
      // Handle disconnection event
      Serial.println("Disconnected from BLE server");
      delete pClient;
   }
};

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
   Serial.print("\nNOTIFY CALLBACK:"); Serial.println((char*)pData);

   char myChar[24] = "";
   strcpy(myChar, (char*)pData);
   // unsigned long dif = millis() - timeRef2;
   // Serial.printf("TIME DIF = %lu", dif);
   Serial.printf(myChar);
   // AppPrintHeap();
   // bleConnected = true;
}

class MyBLEScanCallbacks : public BLEAdvertisedDeviceCallbacks {
public:
   void onResult(BLEAdvertisedDevice advertisedDevice) {
      BLEAddress addr = advertisedDevice.getAddress();
      const char* name = advertisedDevice.getName().c_str();
      const char* servUUID = advertisedDevice.getServiceUUID().toString().c_str();
      const char *servData = advertisedDevice.getServiceDataUUID().toString().c_str();

      Serial.printf("\nName = %s", name);
      Serial.print("\nAddr = "); Serial.print(addr.toString().c_str());
      Serial.printf("\nservUUID = %s", servUUID);
      Serial.printf("\nservDataUUID = %s", servData);
      Serial.println();

      // Check if the device matches the target address
      if (advertisedDevice.getAddress().toString() != TARGET_DEVICE_ADDRESS) return;
      Serial.println("Found target device!");

      // // Stop scanning
      // BLEDevice::getScan()->stop();
      // // BLEAddress *addr = new BLEAddress(advertisedDevice.getAddress());
      // BLEClient* pClient = BLEDevice::createClient();

      // // Connect to the BLE Server.
      // pClient->connect(advertisedDevice.getAddress());
      // Serial.println("===>>>CONNECTED");

      // BLEUUID serviceId = BLEUUID("0000ffe0-0000-1000-8000-00805f9b34fb");
      // BLEUUID characteristicId = BLEUUID("0000ffe1-0000-1000-8000-00805f9b34fb");
      // BLERemoteService* pService = pClient->getService(serviceId);
      // BLERemoteCharacteristic *pCharacteristic = pService->getCharacteristic(characteristicId);
      // pCharacteristic->registerForNotify(notifyCallback);
   }
};


BLEAdvertisedDevice* myDevice;
BLEClient* pClient;
unsigned long timeRef2 = 0;

class Net_Bluetooth {
   BLEScan *pBLEScan;
   BLERemoteCharacteristic* pRemoteCharacteristic;
   BLERemoteCharacteristic* pCharacteristic;
   MyBLEClientCallbacks* pCallbacks = new MyBLEClientCallbacks();

   public:
      void setup() {
         BLEDevice::init("");
         pBLEScan = BLEDevice::getScan(); //create new scan
         pBLEScan->setAdvertisedDeviceCallbacks(new MyBLEScanCallbacks());
         pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
         // pBLEScan->setInterval(500);
         // pBLEScan->setWindow(99); // less or equal setInterval value
         // pBLEScan->start(1, true);
      }

      bool connectToTarget(BLEAdvertisedDevice device) {
         myDevice = new BLEAdvertisedDevice(device);
         pClient = BLEDevice::createClient();
         pClient->setClientCallbacks(pCallbacks);

         if (pClient->connect(myDevice) == false) return false;
         if (pClient->isConnected() == false) return false;

         // std::map<std::string, BLERemoteService*> *services = pClient->getServices();
         // // Print the UUIDs of all services
         // for (auto const& service : *services) {
         //    BLERemoteService* pRemoteService = service.second;
         //    Serial.print("\n______Service:");
         //    Serial.println(pRemoteService->getUUID().toString().c_str());
         //    std::map<std::string, BLERemoteCharacteristic*>* characteristics = pRemoteService->getCharacteristics();

         //    Serial.println("\nList of all Characteristics:");
         //    for (auto const& characteristic : *characteristics) {
         //          Serial.println(characteristic.second->getUUID().toString().c_str());
         //    }
         // }

         Serial.println("===>>>CONNECTED");
         BLEUUID serviceId = BLEUUID("0000ffe0-0000-1000-8000-00805f9b34fb");
         BLEUUID characteristicId = BLEUUID("0000ffe1-0000-1000-8000-00805f9b34fb");
         BLERemoteService* pService = pClient->getService(serviceId);
         pCharacteristic = pService->getCharacteristic(characteristicId);
         pCharacteristic->registerForNotify(notifyCallback);
         delete pClient;

         Serial.println("IM HERE 1");
         bleConnected = true;
         return true;
      }
      
      void scanForDevice2(const char* target, bool filter = false) {
         if (bleConnected) return;

         // Serial.println("\nSTART SCAN ..."); AppPrintHeap();
         // if (myDevice != nullptr) {
         //    pClient->disconnect();
         //    delete myDevice;
         //    delete pClient;
         //    Serial.println("\n===>>>DELETED***");
         // }

         BLEScanResults foundDevices = pBLEScan->start(1, true);
         for (int i=0; i<foundDevices.getCount(); i++) {
            BLEAdvertisedDevice device = foundDevices.getDevice(i);
            BLEAddress addr = device.getAddress();
            const char* name = device.getName().c_str();
            const char* servUUID = device.getServiceUUID().toString().c_str();
            const char *servData = device.getServiceDataUUID().toString().c_str();

            if (!filter) {
               Serial.printf("\nName = %s", name);
               Serial.print("\nAddr = "); Serial.print(addr.toString().c_str());
               Serial.printf("\nservUUID = %s", servUUID);
               Serial.printf("\nservDataUUID = %s", servData);
               Serial.println();
            }

            std::string nameStr = name;
            std::string nameSub = nameStr.substr(0, 4);
            // if (strcmp(nameSub.c_str(), target) != 0) continue;

            BLEAddress compareAddr = BLEAddress(target);
            if (!addr.equals(compareAddr)) continue;
            if (!connectToTarget(device)) continue;
            break;
         }
      }
      
      void scanForDevice(const char* target) {
         BLEScanResults foundDevices = pBLEScan->start(1, true);

         for (int i=0; i<foundDevices.getCount(); i++) {
            BLEAdvertisedDevice device = foundDevices.getDevice(i);
            BLEAddress addr = device.getAddress();
            BLEAddress compareAddr = BLEAddress(target);

            if (addr.equals(compareAddr)) {
               uint8_t* payload = device.getPayload();
               Serial.print("addr = "); Serial.println(device.getAddress().toString().c_str());
               Serial.print("Payload = "); Serial.println(payload[0]);

               std::string strManufacturerData = device.getManufacturerData();
               uint8_t cManufacturerData[100];
               strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

               if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00) {
                  Serial.println("Found an iBeacon!");
                  BLEBeacon oBeacon = BLEBeacon();
                  oBeacon.setData(strManufacturerData);
                  Serial.printf("iBeacon Frame\n");
                  Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
               } else {
                  Serial.println("Found another manufacturers beacon!");
                  Serial.printf("strManufacturerData: %d ", strManufacturerData.length());
                  for (int i = 0; i < strManufacturerData.length(); i++)
                  {
                     Serial.printf("%X", cManufacturerData[i]);
                  }
                  Serial.printf("\n");
               }


               BLEClient* pClient = BLEDevice::createClient();
               myDevice = new BLEAdvertisedDevice(device);
               bool check = pClient->connect(myDevice);
               Serial.print("\n****CHECK = "); Serial.println(check);
               delay(100);
               pClient->disconnect();
            }
         }
      }

         // 98:89:13:0a:4e:36
      void run() {
         BLEScanResults foundDevices = pBLEScan->start(1);
         Serial.print("Devices found: ");
         Serial.println(foundDevices.getCount());
         Serial.println("Scan done!");
         Serial.println("--------------");

         for (int i=0; i<foundDevices.getCount(); i++) {
            BLEAdvertisedDevice device = foundDevices.getDevice(i);
            uint8_t* payload = device.getPayload();

            Serial.print("\nName = ") + Serial.println(device.getName().c_str());
            Serial.print("servUUID = ") + Serial.println(device.getServiceUUID().toString().c_str());
            Serial.print("servDataUUID = ") + Serial.println(device.getServiceDataUUID().toString().c_str());
            BLEAddress addr = device.getAddress();
            BLEAddress compareAddr = BLEAddress("98:89:13:0a:4e:36");

            bool compare = addr.equals(compareAddr);
            Serial.print("****Compare = "); Serial.println(compare);

            if (addr.equals(compareAddr)) {
               Serial.print("addr = "); Serial.println(device.getAddress().toString().c_str());
               Serial.print("Payload = "); Serial.println(payload[0]);

               std::string strManufacturerData = device.getManufacturerData();

               uint8_t cManufacturerData[100];
               strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

               if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00) {
                  Serial.println("Found an iBeacon!");
                  BLEBeacon oBeacon = BLEBeacon();
                  oBeacon.setData(strManufacturerData);
                  Serial.printf("iBeacon Frame\n");
                  Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
               } else {
                  Serial.println("Found another manufacturers beacon!");
                  Serial.printf("strManufacturerData: %d ", strManufacturerData.length());
                  for (int i = 0; i < strManufacturerData.length(); i++)
                  {
                     Serial.printf("%X", cManufacturerData[i]);
                  }
                  Serial.printf("\n");
               }
            }

            // Serial.print("addr = "); Serial.println(device.getAddress().toString().c_str());
            // Serial.print("Payload = "); Serial.println(payload[0]);

            // std::string strManufacturerData = device.getManufacturerData();

            // uint8_t cManufacturerData[100];
            // strManufacturerData.copy((char *)cManufacturerData, strManufacturerData.length(), 0);

            // if (strManufacturerData.length() == 25 && cManufacturerData[0] == 0x4C && cManufacturerData[1] == 0x00) {
            //    Serial.println("Found an iBeacon!");
            //    BLEBeacon oBeacon = BLEBeacon();
            //    oBeacon.setData(strManufacturerData);
            //    Serial.printf("iBeacon Frame\n");
            //    Serial.printf("ID: %04X Major: %d Minor: %d UUID: %s Power: %d\n", oBeacon.getManufacturerId(), ENDIAN_CHANGE_U16(oBeacon.getMajor()), ENDIAN_CHANGE_U16(oBeacon.getMinor()), oBeacon.getProximityUUID().toString().c_str(), oBeacon.getSignalPower());
            // } else {
            //    Serial.println("Found another manufacturers beacon!");
            //    Serial.printf("strManufacturerData: %d ", strManufacturerData.length());
            //    for (int i = 0; i < strManufacturerData.length(); i++)
            //    {
            //       Serial.printf("%X", cManufacturerData[i]);
            //    }
            //    Serial.printf("\n");
            // }
         }

         pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
      }
};