#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>

#include <0Foundation.h>
#include "3Mng_Runtime.h"
#include <Adafruit_INA219.h>

#define PIN_CTRL1 12
#define PIN_CTRL2 14
#define PIN_DONE 13
#define PIN_MODE 14
#define PIN_DRIVE 15
#define PIN_LED 2

enum DeviceMode {
   MODE_LOWPOWER,
   MODE_SETUP
};

Mod2_SHT3 sht;
Mod2_BH17 bh17;
Serv_Tweet tweet;
Serv_EspNow espNow;
Net_Wifi wifi;
TimeoutItem testTimer;       //* print time logger
int BROADCAST_CHANNEL = 10;

Serv_Device device;
Mng_Network network;
DeviceMode currentMode = MODE_LOWPOWER;

std::function<void(DataPacket2*)> onTweet2 = [](DataPacket2* packet) {
   Serial.println("IM HERE 3");
   // espNow.send(packet, sizeof(DataPacket2));
};

unsigned long timeRef, timeDif;
Adafruit_INA219 ina219;

void logSensors() {
   float temp = 0, hum = 0, lux = 0, busvoltage = 0, current_mA = 0;

   // collect readings

   busvoltage = ina219.getBusVoltage_V();
   current_mA = ina219.getCurrent_mA();
   if (std::isnan(current_mA)) current_mA = 0.0f;
   Serial.printf("\nBusVolt = %.2f, curr(mA) = %.2f", busvoltage, current_mA);
   
   sht.collectReadings();
   bh17.collectReadings();
   temp = sht.getTemp();
   hum = sht.getHum();
   lux = bh17.getLux();
   Serial.printf("\ntemp = %.2f, hum = %.2f, lux = %.2f", temp, hum, lux);

   // send readings
   tweet.record.sendTempHumLux(temp, hum, lux, busvoltage, current_mA);
}

void configureESPNow() {
   wifi.setTxPower(0);
   wifi.startAP(true, BROADCAST_CHANNEL);
   tweet.setup(&espNow);
   espNow.setup(WiFi.channel());
   // Serial.print("Channel = "); Serial.println(WiFi.channel());
}

void setup() {
   pinMode(PIN_DRIVE, OUTPUT);
   digitalWrite(PIN_DRIVE, HIGH);            //! Power Lock
   Serial.begin(115200);
   unsigned long timeRef = millis();

   pinMode(PIN_CTRL1, INPUT);    //! Control Input1
   pinMode(PIN_CTRL2, INPUT);    //! Control Input2
   pinMode(PIN_MODE, INPUT_PULLUP);
   pinMode(PIN_DONE, OUTPUT);
   pinMode(PIN_LED, OUTPUT);

   bool read1 = digitalRead(PIN_CTRL1);
   bool read2 = digitalRead(PIN_CTRL2);
   configureESPNow();

   if (read1 || read2) {
      read1 ? tweet.command.sendSingleClick(PIN_CTRL1) : tweet.command.sendSingleClick(PIN_CTRL2);
      delay(1);
      digitalWrite(PIN_DRIVE, LOW);             //! Power Release
      for (;;) {}

   } else {
      // setup i2C
      Wire.begin(4, 5);
      sht.setup(&Wire);
      bh17.setup(&Wire);

      // Serial.print("TWEET MODE");
      if(!ina219.begin()){
         // Serial.println("INA219 not connected!");
      }

      // request readings: need to wait at least 20ms before collect reading
      // start the Wifi to save wait time
      sht.requestReadings();
      bh17.requestReadings();
   }


   // if (!digitalRead(PIN_MODE)) {
   //    Serial.print("SETUP MODE");
   //    device.configure();
   //    network.setup(&device);
   //    currentMode = MODE_SETUP;
   //    return;
   // }
   // Serial.printf("\nTImeDIf = %lu", millis()-timeRef);
   
}

void loop() {
   if (currentMode == MODE_SETUP) {
      network.run();
   } else {
      logSensors();
      digitalWrite(PIN_DONE, !digitalRead(PIN_DONE));
      delay(2000);
   }
}
