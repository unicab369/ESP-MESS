#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "BLEEddystoneURL.h"
#include "BLEEddystoneTLM.h"
#include "BLEBeacon.h"

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

bool isBLEConnected = false;
BLEAdvertisedDevice* myDevice;
BLEClient* pClient;
char myChar[16] = "";

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

void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
   Serial.print("\nNOTIFY CALLBACK:"); Serial.println((char*)pData);
   strcpy(myChar, (char*)pData);
   // digitalWrite(ledPin, HIGH);
   Serial.println(myChar);
}

void getServices(bool listAll = false) {
   if (listAll) {
      std::map<std::string, BLERemoteService*> *services = pClient->getServices();

      // Print the UUIDs of all services
      for (auto const& service : *services) {
         BLERemoteService* pRemoteService = service.second;
         if (pRemoteService == nullptr) continue;
         
         Serial.print("\n***Service:");
         Serial.println(pRemoteService->getUUID().toString().c_str());
         std::map<std::string, BLERemoteCharacteristic*>* characteristics = pRemoteService->getCharacteristics();

         Serial.println("\nList of all Characteristics:");
         for (auto const& characteristic : *characteristics) {
            BLERemoteCharacteristic *const charistic = characteristic.second;
            if (charistic == nullptr) continue;
            Serial.println(characteristic.second->getUUID().toString().c_str());
         }
      }
   }

   BLEUUID serviceId = BLEUUID("0000ffe0-0000-1000-8000-00805f9b34fb");
   BLEUUID characteristicId = BLEUUID("0000ffe1-0000-1000-8000-00805f9b34fb");
   BLERemoteService* pService = pClient->getService(serviceId);

   if (pService != nullptr) {
      BLERemoteCharacteristic* pCharacteristic = pService->getCharacteristic(characteristicId);

      if (pCharacteristic != nullptr) {
         pCharacteristic->registerForNotify(notifyCallback);
      }  
   }
}

class MyBLEClientCallbacks : public BLEClientCallbacks {
public:
   void onConnect(BLEClient* pClient) {
      // Handle connection event
      Serial.println("Connected to BLE server");
      // digitalWrite(ledPin2, HIGH);

      if (pClient->isConnected()) {
            Serial.println("\n===>>>CONNECTED");
            std::map<std::string, BLERemoteService*> *services = pClient->getServices();

            Serial.print("NULL = "); Serial.println(services == nullptr);
            Serial.print("Size = "); Serial.println(services->size());

            // Print the UUIDs of all services
            getServices();
      }
   }

   void onDisconnect(BLEClient* pClient) {
      // Handle disconnection event
      Serial.println("Disconnected from BLE server");
      delete pClient;
      // digitalWrite(ledPin2, LOW);
   }
};

class Net_Bluetooth {
   BLEScan *pBLEScan;
   MyBLEClientCallbacks *clientCb = new MyBLEClientCallbacks();

   public:
      void setup() {
         BLEDevice::init("");
         pBLEScan = BLEDevice::getScan(); //create new scan
         // pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
         pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
         // pBLEScan->setInterval(500);
         // pBLEScan->setWindow(99); // less or equal setInterval value
         // pBLEScan->start(0, true);
         // pinMode(ledPin2, OUTPUT);
         // pinMode(12, OUTPUT);
         // pinMode(13, OUTPUT);
      }
      
      void scanForDevice2(const char* target, bool filter = false) {
         if (isBLEConnected) return;
         // Serial.println("\nSTART SCAN ..."); AppPrintHeap();
         BLEScanResults foundDevices = pBLEScan->start(1, true);
         // digitalWrite(13, !digitalRead(13));

         for (int i=0; i<foundDevices.getCount(); i++) {
            BLEAdvertisedDevice device = foundDevices.getDevice(i);
            const char* addr = device.getAddress().toString().c_str();
            const char* name = device.getName().c_str();
            const char* servId = device.getServiceUUID().toString().c_str();
            const char* servDataId = device.getServiceDataUUID().toString().c_str();

            if (!filter) {
               std::string ignoreName= "JTX-RGB";
               bool blkList = strcmp(name, ignoreName.c_str()) == 0;
               if (blkList) continue;

               ignoreName = "NEMR61622510";
               blkList = strcmp(name, ignoreName.c_str()) == 0;
               if (blkList) continue;

               ignoreName = "ihoment_H6127_9383";
               blkList = strcmp(addr, ignoreName.c_str()) == 0;
               if (blkList) continue;

               // ignoreName = "51:1f:01:5f:bf:0a";
               // blkList = strcmp(addr, ignoreName.c_str()) == 0;
               // if (blkList) continue;

               // ignoreName = "70:09:71:82:4d:c1";
               // blkList = strcmp(addr, ignoreName.c_str()) == 0;
               // if (blkList) continue;

               // ignoreName = "7c:95:6d:8c:23:bb";
               // blkList = strcmp(addr, ignoreName.c_str()) == 0;
               // if (blkList) continue;

               // ignoreName = "11:86:6d:c9:44:a5";
               // blkList = strcmp(addr, ignoreName.c_str()) == 0;
               // if (blkList) continue;

               // ignoreName = "13:0d:e3:50:ad:7b";
               // blkList = strcmp(addr, ignoreName.c_str()) == 0;
               // if (blkList) continue;

               // ignoreName = "4e:d9:a3:84:e2:f3";
               // blkList = strcmp(addr, ignoreName.c_str()) == 0;
               // if (blkList) continue;

               // ignoreName = "23:d5:1a:0e:d3:2a";
               // blkList = strcmp(addr, ignoreName.c_str()) == 0;
               // if (blkList) continue;

               // ignoreName = "43:ca:f0:5d:d4:aa";
               // blkList = strcmp(servId, ignoreName.c_str()) == 0;
               // if (blkList) continue;

               // ignoreName = "4f:09:ca:50:97:77";
               // blkList = strcmp(servId, ignoreName.c_str()) == 0;
               // if (blkList) continue;

               // std::string blkList = "Fencelight";      // Fencelight
               // bool check3 = strcmp(name, blkList.c_str()) != 0;
               // if (!check3) continue;

               // blkList = "bf:27:cc:17:5a:11";                  // TY
               // check3 = strcmp(addr, blkList.c_str()) != 0;
               // if (!check3) continue;

               // blkList = "THWL";
               // check3 = strcmp(name, blkList.c_str()) != 0;
               // if (!check3) continue;

               // blkList = "TY";
               // check3 = strcmp(name, blkList.c_str()) != 0;
               // if (!check3) continue;

               // ignoreName = "<NULL>";
               // blkList = strcmp(servId, ignoreName.c_str()) == 0;
               // if (blkList) continue;

               // digitalWrite(ledPin, !digitalRead(ledPin));
               Serial.printf("\nName = %s", name);
               Serial.print("\nAddr = "); Serial.println(addr);
               Serial.print("servUUID = "); Serial.println(servId);
               Serial.print("servDataUUID = "); Serial.println(servDataId);

               Serial.print("Raw Manufacturer Data: ");
               std::string manufacturerData = device.getManufacturerData();
               for (int i = 0; i < manufacturerData.length(); i++) {
                     Serial.print(manufacturerData[i], HEX);
                     Serial.print(" ");
               }

               Serial.println();
               std::string whiteList = "Temperature";

               BLEAddress compAddr = BLEAddress("98:89:13:0a:69:9b");
               BLEAddress compAddr2 = BLEAddress("b7:7b:08:10:c9:b1");
               bool check4 = device.getAddress().equals(compAddr) || 
                              device.getAddress().equals(compAddr2) ||
                              strcmp(name, whiteList.c_str()) == 0;

               // 0000ffe0-0000-1000-8000-00805f9b34fb
               // bool check4 = strcmp(name, compAddr.c_str()) == 0;

               pClient = BLEDevice::createClient();
               // pClient->setClientCallbacks(clientCb);

               if (check4 && pClient->connect(&device)) {
                  Serial.println("Connected to the device");
                  getServices(true);

                  // Disconnect from the device
                  pClient->disconnect();
                  // Serial.println("Disconnected from the device");

                  // isBLEConnected = true;
                  // pBLEScan->stop();
                  // pBLEScan->clearResults();
                  break;
               }
            }
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
};