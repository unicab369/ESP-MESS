#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>

#include <0Foundation.h>
#include "3Mng_Runtime.h"

Mng_Runtime runTime;

void setup() {
   Serial.begin(115200);
   runTime.setupRunTime();
}

void loop() {
   runTime.runJob1();
   runTime.runJob2();
}