#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
float temp = 0, hum = 0, lux = 0, busvoltage = 0, current_mA = 0;

void setup() {
   Serial.begin(115200);
   Serial.println(__FILE__);

   Wire.begin(4, 5);

   if(!ina219.begin()){
      Serial.println("INA219 not connected!");
   }
}

void loop() {
   busvoltage = ina219.getBusVoltage_V();
   current_mA = ina219.getCurrent_mA();

   if (abs(current_mA)<1) {
      busvoltage = 0;
   }

   Serial.printf("\nBusVolt = %.2f, curr(mA) = %.2f", busvoltage, current_mA);
   delay(500);
}
