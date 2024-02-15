#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>

#include <0Foundation.h>
#include "3Mng_Runtime.h"

Mng_Runtime runTime;
// Net_Bluetooth bluetooth;
// Disp_SSD13062 disp;
// int counter = 0;
// Disp_Epaper epaperz;

void setup() {
  Serial.begin(115200);
  runTime.setupRunTime();

  // // SPI.begin(18, 19, 23, 2);
  // // epaperz.setup();
  // bluetooth.setup();
  // Wire.begin(4, 5);
  // disp.setup(&Wire);
  // pinMode(12, OUTPUT);

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
      //! should not be called for ESP32 duo cores
      runTime.runJob1();
      runTime.runJob2();
      // bluetooth.connectToDevice("JDY-", true);
      // String output = "count="+String(counter);
      // disp.printline(myChar, 0);
      // disp.printline(output, 1);
      // digitalWrite(12, !digitalRead(12));
      // counter++;
    }
  #endif
} 

void loop() {
  //! should not be called
  // epaperz.test();
}