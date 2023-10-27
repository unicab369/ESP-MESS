#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>
// #include <0Foundation.h>
// #include "3Mng_Runtime.h"

#define UNIT_TEST 1

// Mng_Runtime runTime;

#ifdef UNIT_TEST
  #include "examples/AppTest.h"

#else
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