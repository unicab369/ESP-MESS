#include <Arduino.h>
#include <functional>
// #include <Wire.h>
// #include <SPI.h>

#include <0Foundation.h>
#include "3Mng_Runtime.h"

Mng_Runtime runTime;
// Net_Bluetooth bluetooth;
// Disp_SSD13062 disp;
// int counter = 0;
// Disp_Epaper epaperz;

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  runTime.setupRunTime();

  // // // SPI.begin(18, 19, 23, 2);
  // // // epaperz.setup();
  // // bluetooth.setup();
  // // Wire.begin(4, 5);
  // // disp.setup(&Wire);
  // // pinMode(12, OUTPUT);
  
  xTaskCreatePinnedToCore([](void *pvParam) {
    while(1) { 
      runTime.runJob1();
    }
  }, "Core0_TaskA", 10000, NULL, 0, NULL, 0);

  xTaskCreatePinnedToCore([](void *pvParam) {
    while(1) {
      runTime.runJob2();
    }
  }, "Core1_TaskA", 10000, NULL, 1, NULL, 1);
} 

void loop() {
  //! should not be called
  // epaperz.test();
}