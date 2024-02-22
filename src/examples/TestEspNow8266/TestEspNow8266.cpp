#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>

#include <0Foundation.h>
#include "3Mng_Runtime.h"
#include <Adafruit_INA219.h>

#define DONE_PIN 13
#define MODE_PIN 14
#define LED_PIN 2

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

void setup() {
   unsigned long timeRef = millis();

   pinMode(DONE_PIN, OUTPUT);
   pinMode(LED_PIN, OUTPUT);
   pinMode(MODE_PIN, INPUT_PULLUP);

   Serial.begin(115200);

   if (!digitalRead(MODE_PIN)) {
      Serial.print("SETUP MODE");
      device.configure();
      network.setup(&device);
      currentMode = MODE_SETUP;
      return;
   }

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

   wifi.setTxPower(0);
   wifi.startAP(true, BROADCAST_CHANNEL);
   // tweet.setup(&device, espNow.mac, &onTweet2);
   espNow.setup(WiFi.channel());

   // Serial.print("Channel = "); Serial.println(WiFi.channel());
   // Serial.printf("\nTImeDIf = %lu", millis()-timeRef);
   
}

void loop() {
   if (currentMode == MODE_SETUP) {
      network.run();
   } else {
      logSensors();
      digitalWrite(DONE_PIN, !digitalRead(DONE_PIN));
      delay(2000);
   }
}
