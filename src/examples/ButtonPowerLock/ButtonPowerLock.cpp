#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>


void setup() {
   pinMode(2, OUTPUT);
   digitalWrite(2, HIGH);
   Serial.begin(115200);
}

void loop() {
   Serial.println("IM HERE uuu");
   delay(2000);
   digitalWrite(2, !digitalRead(2));
}