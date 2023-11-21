#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>
#include <0Foundation.h>
#include "3Mng_Runtime.h"

#define MODE_TEST 1
// #define MODE_SLEEP 1

#ifdef MODE_TEST
  #include "examples/AppTest.h"

#elif defined(MODE_SLEEP)
  #include <Adafruit_INA219.h>
  Mod2_SHT3 sht;
  Mod2_BH17 bh17;
  Serv_Tweet tweet;
  Serv_EspNow espNow;
  Serv_Device device;
  Net_Wifi wifi;
  TimeoutItem testTimer;       //* print time logger
  int BROADCAST_CHANNEL = 2;

  std::function<void(DataPacket2*)> onTweet2 = [](DataPacket2* packet) {
    Serial.println("IM HERE 3");
    espNow.send(packet, sizeof(DataPacket2));
  };

  unsigned long timeRef, timeDif;

  Adafruit_INA219 ina219;

  void setup() {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    Serial.begin(115200);

    // setup i2C
    Wire.begin(4, 5);
    sht.setup(&Wire);
    bh17.setup(&Wire);

    if(!ina219.begin()){
      Serial.println("INA219 not connected!");
    }

    // request readings: need to wait at least 20ms before collect reading
    // start the Wifi to save wait time
    sht.requestReadings();
    bh17.requestReadings();
    // delay(20);   //! this is needed if wifi is not used

    wifi.setTxPower(20);
    wifi.startAP(true, BROADCAST_CHANNEL);
    tweet.setup(&device, espNow.mac, &onTweet2);
    Serial.print("Channel = "); Serial.println(WiFi.channel());
    espNow.setup(WiFi.channel());

    // collect readings
    sht.collectReadings();
    bh17.collectReadings();
    float temp = sht.getTemp();
    float hum = sht.getHum();
    float lux = bh17.getLux();
    Serial.printf("\ntemp = %.2f, hum = %.2f, lux = %.2f", temp, hum, lux);

    float busvoltage = ina219.getBusVoltage_V();
    float current_mA = ina219.getCurrent_mA();
    Serial.printf("\nBusVolt = %.2f, curr(mA) = %.2f", busvoltage, current_mA);

    // send readings
    tweet.record.sendTempHumLux(temp, hum, lux, busvoltage, current_mA);
    ESP.deepSleep(3e6);
  }

  void loop() {
  }

#else
  Mng_Runtime runTime;

  void setup() {
    Serial.begin(115200);
    runTime.setupRunTime();

    #if defined(ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
      xTaskCreatePinnedToCore([](void *pvParam) {
        for (;;) { 
          runTime.runJob1();
        }
      }, "loopCore0", 10000, NULL, 1, NULL, 0);

      xTaskCreatePinnedToCore([](void *pvParam) {
        for (;;) { 
          runTime.runJob2();
        }
      }, "loopCore1", 5000, NULL, 1, NULL, 1);
    #else
      for(;;) {
        //! should not be called for ESP32
        runTime.runJob1();
        runTime.runJob2();
      }
    #endif
  } 

  void loop() {
    //! should not be called
  }
#endif