#ifdef CONFIG_IDF_TARGET_ESP32C3
   uint8_t ledPin = 12;
#elif defined(ESP32)
   uint8_t ledPin = 22;
#else
   uint8_t ledPin = 2;
#endif

// #define TEST_PCA96 1
// #define TEST_BEHAVIOR 1
// #define TEST_PWM 1
// #define TEST_AUDIO 1
#define TEST_BLINK 1
// #define TEST_MICROPHONE 1
// #define TEST_BLE 1
// #define TEST_BLE_BEACON 1
// #define TEST_BLE5 1
// #define TEST_BLE_SCANNER 1

Loggable TestLog("Test"); 
Mng_Config conf;
MyButton button1;
RotaryEncoder rotary;
SerialControl serial;

void testSetup() {
   conf.setup();
   Wire.begin(conf.sda0, conf.scl0);
}

void testRun() {
   button1.run();
   rotary.run();
   serial.run();
}

#ifdef TEST_BLINK
   void setup() {
      Serial.begin(115200);
      AppPrint("\nIM HERE 000");
      pinMode(ledPin, OUTPUT);
      // pinMode(13, OUTPUT);
      // pinMode(5, OUTPUT);
   }

   void loop() {
      TestLog.xLog("\nIM HERE 11122");
      uint8_t read = digitalRead(ledPin);
      digitalWrite(ledPin, !read);
      // digitalWrite(13, read);
      // digitalWrite(5, read);
      delay(500);
   }

#elif TEST_BLE_SCANNER
   #include <Net_Bluetooth.h>

   Net_Bluetooth bluetooth;

   void setup() {
      Serial.begin(115200);
      Serial.println("Scanning...");
      AppPrint("IM HERE 000");
      pinMode(ledPin, OUTPUT);

      bluetooth.setup();
   }

   void loop() {
      // put your main code here, to run repeatedly:
      // Serial.println("\nIM HERE zzz");
      bluetooth.scanForDevice("98:89:13:0a:4e:36");
      // bluetooth.run();

      // delay(1000);
      // uint8_t read = digitalRead(ledPin);
      // digitalWrite(ledPin, !read);
   }

#elif TEST_BLE5
   #include <BLEDevice.h>
   #include <BLEAdvertising.h>

   esp_ble_gap_ext_adv_params_t ext_adv_params_1M = {
      .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_CONNECTABLE,
      .interval_min = 0x30,
      .interval_max = 0x30,
      .channel_map = ADV_CHNL_ALL,
      .own_addr_type = BLE_ADDR_TYPE_RANDOM,
      .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
      .primary_phy = ESP_BLE_GAP_PHY_CODED,
      .max_skip = 0,
      .secondary_phy = ESP_BLE_GAP_PHY_1M,
      .sid = 0,
      .scan_req_notif = false,
   };

   esp_ble_gap_ext_adv_params_t ext_adv_params_2M = {
      .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_SCANNABLE,
      .interval_min = 0x40,
      .interval_max = 0x40,
      .channel_map = ADV_CHNL_ALL,
      .own_addr_type = BLE_ADDR_TYPE_RANDOM,
      .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
      .primary_phy = ESP_BLE_GAP_PHY_1M,
      .max_skip = 0,
      .secondary_phy = ESP_BLE_GAP_PHY_2M,
      .sid = 1,
      .scan_req_notif = false,
   };

   esp_ble_gap_ext_adv_params_t legacy_adv_params = {
      .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY_IND,
      .interval_min = 0x45,
      .interval_max = 0x45,
      .channel_map = ADV_CHNL_ALL,
      .own_addr_type = BLE_ADDR_TYPE_RANDOM,
      .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
      .primary_phy = ESP_BLE_GAP_PHY_1M,
      .max_skip = 0,
      .secondary_phy = ESP_BLE_GAP_PHY_1M,
      .sid = 2,
      .scan_req_notif = false,
   };

   esp_ble_gap_ext_adv_params_t ext_adv_params_coded = {
      .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_SCANNABLE,
      .interval_min = 0x50,
      .interval_max = 0x50,
      .channel_map = ADV_CHNL_ALL,
      .own_addr_type = BLE_ADDR_TYPE_RANDOM,
      .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
      .primary_phy = ESP_BLE_GAP_PHY_1M,
      .max_skip = 0,
      .secondary_phy = ESP_BLE_GAP_PHY_CODED,
      .sid = 3,
      .scan_req_notif = false,
   };

   static uint8_t raw_adv_data_1m[] = {
      0x02, 0x01, 0x06,
      0x02, 0x0a, 0xeb,
      0x12, 0x09, 'E', 'S', 'P', '_', 'M', 'U', 'L', 'T', 'I', '_', 'A',
      'D', 'V', '_', '1', 'M', 0X0
   };

   static uint8_t raw_scan_rsp_data_2m[] = {
      0x02, 0x01, 0x06,
      0x02, 0x0a, 0xeb,
      0x12, 0x09, 'E', 'S', 'P', '_', 'M', 'U', 'L', 'T', 'I', '_', 'A',
      'D', 'V', '_', '2', 'M', 0X0
   };

   static uint8_t legacy_adv_data[] = {
      0x02, 0x01, 0x06,
      0x02, 0x0a, 0xeb,
      0x15, 0x09, 'E', 'S', 'P', '_', 'M', 'U', 'L', 'T', 'I', '_', 'A',
      'D', 'V', '_', 'C', 'O', 'D', 'E', 'D', 0X0
   };
   
   static uint8_t legacy_scan_rsp_data[] = {
      0x02, 0x01, 0x06,
      0x02, 0x0a, 0xeb,
      0x16, 0x09, 'E', 'S', 'P', '_', 'M', 'U', 'L', 'T', 'I', '_', 'A',
      'D', 'V', '_', 'L', 'E', 'G', 'A', 'C', 'Y', 0X0
   };

   static uint8_t raw_scan_rsp_data_coded[] = {
      0x37, 0x09, 'V', 'E', 'R', 'Y', '_', 'L', 'O', 'N', 'G', '_', 'D', 'E', 'V', 'I', 'C', 'E', '_', 'N', 'A', 'M', 'E', '_',
      'S', 'E', 'N', 'T', '_', 'U', 'S', 'I', 'N', 'G', '_', 'E', 'X', 'T', 'E', 'N', 'D', 'E', 'D', '_', 'A', 'D', 'V', 'E', 'R', 'T', 'I', 'S', 'I', 'N', 'G', 0X0
   };


   uint8_t addr_1m[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x01};
   uint8_t addr_2m[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x02};
   uint8_t addr_legacy[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x03};
   uint8_t addr_coded[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x04};

   BLEMultiAdvertising advert(4); // max number of advertisement data 

   void setup() {
      Serial.begin(115200);
      Serial.println("Multi-Advertising...");

      BLEDevice::init("");

      advert.setAdvertisingParams(0, &ext_adv_params_1M);
      advert.setAdvertisingData(0, sizeof(raw_adv_data_1m), &raw_adv_data_1m[0]);
      advert.setInstanceAddress(0, addr_1m);
      advert.setDuration(5);

      // advert.setAdvertisingParams(1, &ext_adv_params_2M);
      // advert.setScanRspData(1, sizeof(raw_scan_rsp_data_2m), &raw_scan_rsp_data_2m[0]);
      // advert.setInstanceAddress(1, addr_2m);
      // advert.setDuration(1);

      // advert.setAdvertisingParams(2, &legacy_adv_params);
      // advert.setAdvertisingData(2, sizeof(legacy_adv_data), &legacy_adv_data[0]);
      // advert.setScanRspData(2, sizeof(legacy_scan_rsp_data), &legacy_scan_rsp_data[0]);
      // advert.setInstanceAddress(2, addr_legacy);
      // advert.setDuration(2);

      // advert.setAdvertisingParams(3, &ext_adv_params_coded);
      // advert.setDuration(3);
      // advert.setScanRspData(3, sizeof(raw_scan_rsp_data_coded), &raw_scan_rsp_data_coded[0]);
      // advert.setInstanceAddress(3, addr_coded);

      delay(1000);
      advert.start(1, 0);
   }

   void loop() {
      delay(4000);
      Serial.println("IM HERE");
   }

#elif TEST_BLE_BEACON
   #include <BLEDevice.h>
   #include <BLEUtils.h>
   #include <BLEServer.h>
   #include <BLEBeacon.h>
   #include "esp_sleep.h"

   #define GPIO_DEEP_SLEEP_DURATION     3  // sleep x seconds and then wake up
   RTC_DATA_ATTR static time_t last;        // remember last boot in RTC Memory
   RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory

   BLEAdvertising *pAdvertising;   // BLE Advertisement type
   #define BEACON_UUID "87b99b2c-90fd-11e9-bc42-526af7764f64" // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)

   void setBeacon() {
      BLEBeacon oBeacon = BLEBeacon();
      oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
      oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
      oBeacon.setMajor((bootcount & 0xFFFF0000) >> 16);
      oBeacon.setMinor(bootcount & 0xFFFF);

      BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
      BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
      oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04

      std::string strServiceData = "a";
      strServiceData += (char)26;     // Len
      strServiceData += (char)0xFF;   // Type
      strServiceData += oBeacon.getData();
      oAdvertisementData.addData(strServiceData);
      pAdvertising->setAdvertisementData(oAdvertisementData);
      pAdvertising->setScanResponseData(oScanResponseData);
   }

   void setup() {
      Serial.begin(115200);

      Serial.printf("start ESP32 %d\n", bootcount++);

      // Create the BLE Device
      BLEDevice::init("ESP32 as iBeacon");
      pinMode(12, INPUT);
      // digitalWrite(12, LOW);
      // delay(200);
      // digitalWrite(12, HIGH);

      // delay(1000);

      // Create the BLE Server
      BLEServer *pServer = BLEDevice::createServer(); // <-- no longer required to instantiate BLEServer, less flash and ram usage
      pAdvertising = BLEDevice::getAdvertising();
      BLEDevice::startAdvertising();
      setBeacon();

      // Start advertising
      pAdvertising->start();
      Serial.println("Advertizing started...");
      delay(100);
      pAdvertising->stop();
      Serial.printf("enter deep sleep\n");
      esp_deep_sleep(1000000LL * GPIO_DEEP_SLEEP_DURATION);
      Serial.printf("in deep sleep\n");
   }

   void loop() {

   }

#elif TEST_BLE
   #include <BLEDevice.h>
   #include <BLEServer.h>
   #include <BLEUtils.h>
   #include <BLE2902.h>

   BLEServer* pServer = NULL;
   BLECharacteristic* pCharacteristic = NULL;
   BLEDescriptor *pDescr;
   BLE2902 *pBLE2902;

   bool deviceConnected = false;
   bool oldDeviceConnected = false;
   uint32_t value = 0;

   // See the following for generating UUIDs:
   // https://www.uuidgenerator.net/

   #define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
   #define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

   class MyServerCallbacks: public BLEServerCallbacks {
      void onConnect(BLEServer* pServer) {
         deviceConnected = true;
      };

      void onDisconnect(BLEServer* pServer) {
         deviceConnected = false;
      }
   };

   void setup() {
      Serial.begin(115200);

      // Create the BLE Device
      BLEDevice::init("ESP32");

      // Create the BLE Server
      pServer = BLEDevice::createServer();
      pServer->setCallbacks(new MyServerCallbacks());

      // Create the BLE Service
      BLEService *pService = pServer->createService(SERVICE_UUID);

      // Create a BLE Characteristic
      pCharacteristic = pService->createCharacteristic(
                           CHARACTERISTIC_UUID,
                           BLECharacteristic::PROPERTY_NOTIFY
                        );                   

      // Create a BLE Descriptor
      
      pDescr = new BLEDescriptor((uint16_t)0x2901);
      pDescr->setValue("A very interesting variable");
      pCharacteristic->addDescriptor(pDescr);
      
      pBLE2902 = new BLE2902();
      pBLE2902->setNotifications(true);
      pCharacteristic->addDescriptor(pBLE2902);

      // Start the service
      pService->start();

      // Start advertising
      BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
      pAdvertising->addServiceUUID(SERVICE_UUID);
      pAdvertising->setScanResponse(false);
      pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
      BLEDevice::startAdvertising();
      Serial.println("Waiting a client connection to notify...");
   }

   void loop() {
      // notify changed value
      if (deviceConnected) {
         pCharacteristic->setValue(value);
         pCharacteristic->notify();
         value++;
         delay(10000);
      }
      // disconnecting
      if (!deviceConnected && oldDeviceConnected) {
         delay(500); // give the bluetooth stack the chance to get things ready
         pServer->startAdvertising(); // restart advertising
         Serial.println("start advertising");
         oldDeviceConnected = deviceConnected;
      }
      // connecting
      if (deviceConnected && !oldDeviceConnected) {
         // do stuff here on connecting
         oldDeviceConnected = deviceConnected;
      }
   }

#elif TEST_MICROPHONE
   #include "AudioTools.h"
   
   void setup() {
      Serial.begin(115200);
      AppPrint("IM HERE 000");
      pinMode(ledPin, OUTPUT);
   }

   void loop() {
      AppPrint("IM HERE 111");
      uint8_t read = digitalRead(12);
      digitalWrite(ledPin, !read);
      delay(1000);
   }

#elif defined(TEST_PCA96)
   PCA96Controller pca96z;

   //! ButtonPress Callback
   std::function<void(BTN_Action, BNT_Hold, uint32_t)> buttonCb = 
                     [&](BTN_Action action, BNT_Hold hold, uint32_t elapse) {
      TestLog.xLog("Button Pressed = %u", action);

      switch (action) {
         case ACTION_SINGLE_CLICK: {
               ControlOutput action1(11, 22);    
               break;
         }   
         case ACTION_DOUBLE_CLICK: {
               break;
         }
         case ACTION_PRESS_ACTIVE: {
               break;
         }
         case ACTION_PRESS_END: {
               break;
         }
      }
   };

   //! RotaryEncoder Callback
   std::function<void(RotaryDirection, uint16_t)> rotaryCb = [&](RotaryDirection state, uint16_t counter) {
      TestLog.xLog("Rotary counter = %llu", counter);
      pca96z.drivePWM(0, counter);
      // pca96z.drivePWM(1, counter);
   };

   void setup() {
      Serial.begin(115200);
      testSetup();

      pca96z.setup(&Wire);
      button1.setup(conf.btn1);
      button1.callback = &buttonCb;

      Serial.print("RotA = "); Serial.println(conf.rotaryA);
      Serial.print("RotB = "); Serial.println(conf.rotaryB);
      
      rotary.setup(conf.rotaryA, conf.rotaryB, 10, { 90, 500 });
      rotary.onCallback = &rotaryCb;
   }

   void loop() {
      // pca96z.test();
      testRun();
   }

#elif defined(TEST_AUDIO)
   #include "MusicDefinitions.h"
   #include "SoundData.h"
   #include "XT_DAC_Audio.h"

   int8_t PROGMEM TwinkleTwinkle[] = {
      NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
      NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_2,
      NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
      NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
      NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
      NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_4,  
      NOTE_SILENCE,BEAT_5,SCORE_END
   };


   XT_Wav_Class ForceWithYou(Force);  
   XT_DAC_Audio_Class DacAudio(25,0);    // Use GPIO 25, one of the 2 DAC pins and timer 0
   XT_MusicScore_Class Music(TwinkleTwinkle,TEMPO_ALLEGRO,INSTRUMENT_PIANO);

   void setup() {
      Serial.begin(115200);
      DacAudio.Play(&Music); 
   }

   void loop() {
      DacAudio.FillBuffer();                // Fill the sound buffer with data
      // if(ForceWithYou.Playing==false)       // if not playing,
      //    // DacAudio.Play(&ForceWithYou);       // play it, this will cause it to repeat and repeat...
      //    DacAudio.Play(&Music); 
   }

#elif defined(TEST_BEHAVIOR)
   Mng_Storage storage;
   Serv_Behavior servBehav;

   uint8_t peer1Mac[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x00};

   //! ButtonPress Callback
   std::function<void(BTN_Action, BNT_Hold, uint32_t)> buttonCb = 
                     [&](BTN_Action action, BNT_Hold hold, uint32_t elapse) {
      switch (action) {
         case ACTION_SINGLE_CLICK: {
               ControlOutput action1(11, 22);    
               servBehav.storeAction<TRIGGER_SINGLECLICK>(0, &action1, peer1Mac);
               break;
         }   
         case ACTION_DOUBLE_CLICK: {
               break;
         }
         case ACTION_PRESS_ACTIVE: {
               break;
         }
         case ACTION_PRESS_END: {
               servBehav.deleteData();
               break;
         }
      }
   };

   //! RotaryEncoder Callback
   std::function<void(RotaryDirection, uint16_t)> rotaryCb = [&](RotaryDirection state, uint16_t counter) {
      uint8_t value = (state == CLOCKWISE) ? 1 : (state == COUNTERCLOCKWISE) ? -1 : 0;
      peer1Mac[5] = peer1Mac[5] + value;
      Serial.print("Update Peer1Mac: ");
      AppPrintHex(peer1Mac, sizeof(peer1Mac));
   };

   std::function<void(char*)> storeCredCb = [&](char* inputStr) {
      storage.handleConsoleStr(inputStr);
   };

   void setup() {
      Serial.begin(115200);

      testSetup();
      storage.setup();
      servBehav.setup();

      // servBehav.stoPeer.printAllPeers();
      servBehav.stoBehav.printAll();      

      // uint8_t peer1Mac[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x07};
      // ControlOutput action1(11, 22);    
      // // servBehav.storeAction<TRIGGER_SINGLECLICK>(0, &action1, peer1Mac);
      // servBehav.stoPeer.addPeer(peer1Mac);

      // ControlWS2812 action2(33, 44);
      // servBehav.storeAction<TRIGGER_DOUBLECLICK>(1, &action2, &peer1);


      button1.setup(conf.btn1, &buttonCb);
      serial.onParseString = &storeCredCb;

      rotary.onCallback = &rotaryCb;
   }

   void loop() {
      testRun();
   }

#elif defined(TEST_PWM)
   PulseController led;

   void setup() {
      Serial.begin(115200);
      led.setup(ledPin, false);
      // led.singlePulses(100);
      // led.doublePulses(100);
      // led.pulse20ms();
      led.uniformPulse500ms();
   }

   void loop() {
      led.run();
   }
#else
   void donothing() {

   }
#endif
