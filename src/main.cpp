#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>
#include <0Foundation.h>
// #include "3Mng_Runtime.h"

// #define MODE_TEST 1
#define MODE_SLEEP 1

#ifdef MODE_TEST
  #include "examples/AppTest.h"

#elif defined(MODE_SLEEP)
  Mod2_SHT3 sht;
  Mod2_BH17 bh17;
  Serv_Tweet tweet;
  Serv_EspNow espNow;
  Serv_Device device;
  Net_Wifi wifi;

  std::function<void(DataPacket2*)> onTweet2 = [](DataPacket2* packet) {
      espNow.send(packet, sizeof(DataPacket2));
  };

  void setup() {
    Serial.begin(115200);
    Serial.println("\n\nIM HERE");

    // WiFi.setOutputPower(0);
    wifi.startAP(false, 11);
    espNow.setup(11);
    tweet.setup(&device, espNow.mac, &onTweet2);
    Wire.begin(4, 5);
    sht.setup(&Wire);
    bh17.setup(&Wire);
  }

  void loop() {
    sht.requestReadings();
    bh17.requestReadings();
    delay(20);
    sht.collectReadings();
    float temp = sht.getTemp();
    float hum = sht.getHum();
    bh17.collectReadings();
    float lux = bh17.getLux();
    Serial.printf("temp = %.2f, hum = %.2f, lux = %.2f", temp, hum, lux);
    // tweet.record.sendTempHumLux(temp, hum, lux);
    ESP.deepSleep(3e6);
  }

#else
  Mng_Runtime runTime;

  void setup() {
    Serial.begin(115200);
    runTime.setup();

    #ifdef ESP32
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
    #endif
  } 

  void loop() {
    #ifndef ESP32
      // should not be called for ESP32
      runTime.runJob1();
      runTime.runJob2();
    #endif
  }
#endif