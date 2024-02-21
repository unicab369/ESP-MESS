#include <Arduino.h>
#include <functional>

int loop1Counter = 0;
int loop2Counter = 0;
uint32_t timeRef1, timeRef2;

void setup() {
   Serial.begin(115200);
   timeRef1 = millis();
   timeRef2 = millis();

   // test number: 953657
   xTaskCreatePinnedToCore([](void *pvParam) {
      for (;;) { 
         loop1Counter++;
         
         if (millis()-timeRef1 > 1000) {
            Serial.printf("\nloop1Counter = %lu", loop1Counter);
            timeRef1 = millis();
            loop1Counter = 0;
         }
      }
   }, "loopCore0", 10000, NULL, 1, NULL, 0);

   // test number: 447045
   xTaskCreatePinnedToCore([](void *pvParam) {
      for (;;) { 
         loop2Counter++;

         if (millis()-timeRef2 > 1000) {
            Serial.printf("\nloop2Counter = %lu", loop2Counter);
            timeRef2 = millis();
            loop2Counter = 0;
         }
      }
   }, "loopCore1", 5000, NULL, 1, NULL, 1);
} 

void loop() {
  //! should not be called
}