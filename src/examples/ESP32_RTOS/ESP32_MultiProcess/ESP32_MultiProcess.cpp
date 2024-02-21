#include <Arduino.h>
#include <functional>

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

int loop1Counter = 0;
int loop2Counter = 0;

void setup() {
   Serial.begin(115200);
   pinMode(22, OUTPUT);
   pinMode(14, INPUT);
   
   //! CORE0 = 1759625
   xTaskCreatePinnedToCore([](void *pvParam) {
      while(1) {
         vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1000 milliseconds

         Serial.printf("\n\nCore0 Timer = %lu", loop1Counter);
         loop1Counter = 0;

         TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
         TIMERG0.wdt_feed=1;
         TIMERG0.wdt_wprotect=0;
      }
   }, "Core0_TaskA", 2048, NULL, 1, NULL, 0);
   
   // Create Task 2 on Core 0
   xTaskCreatePinnedToCore([](void *pvParam) { 
      while(1) {
         // Serial.printf("\nCore0 is running");
         loop1Counter++;
         digitalWrite(22, !digitalRead(22));
      }
   }, "Core0_TaskB", 2048, NULL, 1, NULL, 0);


   //! CORE1 = 2182938
   xTaskCreatePinnedToCore([](void *pvParam) { 
      while(1) {
         vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 2000 milliseconds

         Serial.printf("\n\nCore1** Timer = %lu", loop2Counter);
         loop2Counter = 0;

         TIMERG1.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
         TIMERG1.wdt_feed=1;
         TIMERG1.wdt_wprotect=0;
      }
   }, "Core1_TaskA", 2048, NULL, 1, NULL, 1);

   // Create Task 2 on Core 1
   xTaskCreatePinnedToCore([](void *pvParam) {
      while(1) {
         // Serial.printf("\nCore1** is running");
         loop2Counter++;
         int read = digitalRead(14);
      }
   }, "Core1_TaskB", 2048, NULL, 1, NULL, 1);
}

void loop() {

}