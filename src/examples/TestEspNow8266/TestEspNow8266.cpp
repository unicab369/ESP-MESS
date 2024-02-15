#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>

#include <0Foundation.h>
#include "3Mng_Runtime.h"
#include <Adafruit_INA219.h>

Mod2_SHT3 sht;
Mod2_BH17 bh17;
Serv_Tweet tweet;
Serv_EspNow espNow;
Net_Wifi wifi;
TimeoutItem testTimer;       //* print time logger
int BROADCAST_CHANNEL = 10;

std::function<void(DataPacket2*)> onTweet2 = [](DataPacket2* packet) {
   Serial.println("IM HERE 3");
   espNow.send(packet, sizeof(DataPacket2));
};

unsigned long timeRef, timeDif;
Adafruit_INA219 ina219;

Mng_Runtime runTime;
Serv_Device device;

void logSensors() {
   float temp = 0, hum = 0, lux = 0, busvoltage = 0, current_mA = 0;

   // // collect readings
   // sht.collectReadings();
   // bh17.collectReadings();
   // temp = sht.getTemp();
   // hum = sht.getHum();
   // lux = bh17.getLux();
   Serial.printf("\ntemp = %.2f, hum = %.2f, lux = %.2f", temp, hum, lux);

   // busvoltage = ina219.getBusVoltage_V();
   // current_mA = ina219.getCurrent_mA();
   // Serial.printf("\nBusVolt = %.2f, curr(mA) = %.2f", busvoltage, current_mA);

   // send readings
   delay(10);
   tweet.record.sendTempHumLux(temp, hum, lux, busvoltage, current_mA);
}

void setup() {
   unsigned long timeRef = millis();

   pinMode(2, OUTPUT);
   pinMode(15, INPUT);
   // digitalWrite(13, HIGH);
   Serial.begin(115200);

   if (digitalRead(15)) {
      Serial.print("SETUP MODE");
      runTime.setupRunTime();
      return;
   }

   // setup i2C
   Wire.begin(4, 5);
   // Wire.begin(33, 32);
   sht.setup(&Wire);
   bh17.setup(&Wire);

   Serial.print("TWEET MODE");
   if(!ina219.begin()){
      Serial.println("INA219 not connected!");
   }

   // request readings: need to wait at least 20ms before collect reading
   // start the Wifi to save wait time
   sht.requestReadings();
   bh17.requestReadings();
   // delay(20);   //! this is needed if wifi is not used

   wifi.setTxPower(0);
   wifi.startAP(true, BROADCAST_CHANNEL);
   tweet.setup(&device, espNow.mac, &onTweet2);

   Serial.print("Channel = "); Serial.println(WiFi.channel());
   espNow.setup(WiFi.channel());

   Serial.printf("\nTImeDIf = %lu", millis()-timeRef);
}

void loop() {
   if (digitalRead(15)) {
      runTime.runJob1();
      runTime.runJob2();
      return;
   }

   logSensors();
   // ESP.deepSleep(3e6);
   delay(1);
   digitalWrite(2, !digitalRead(2));
   delay(1000);
}
