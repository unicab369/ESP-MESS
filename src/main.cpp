#include <0Foundation.h>
#include "1Mng_Device.h"    // +11.5%

Mng_Device device;

// #define UNIT_TEST true

#ifdef UNIT_TEST
  #include "AppTest/AppTest.h"

#else
  void setup() {
    Serial.begin(115200);
    device.setup();

    #ifdef ESP32
      xTaskCreatePinnedToCore([](void *pvParam) {
        for (;;) { 
          device.runtime.runJob1(); 
        }
      }, "loopCore0", 10000, NULL, 1, NULL, 0);

      xTaskCreatePinnedToCore([](void *pvParam) {
        for (;;) { 
          device.runtime.runJob2(); 
        }
      }, "loopCore1", 5000, NULL, 1, NULL, 1);
    #endif
  }

  void loop() {
    #ifndef ESP32
      // should not be called for ESP32
      device.runtime.runJob1();
      device.runtime.runJob2();
    #endif
  }
#endif