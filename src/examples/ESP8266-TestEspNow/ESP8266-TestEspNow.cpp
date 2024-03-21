// pio run -e ESP8266-TestEspNow -t upload --upload-port COM4

#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>

#include <0Foundation.h>
#include "3Mng_Runtime.h"
#include <Adafruit_INA219.h>

#include <APDS9930.h>

// Global Variables
APDS9930 apds = APDS9930();
float ambient_light = 0; // can also be an unsigned long
uint16_t ch0 = 0;
uint16_t ch1 = 1;

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

Mod2_SHT3 sht3x;
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

Mod_APDS9930 apds99;

void logSensors() {
   float temp = 0, hum = 0, lux1 = 0, lux2 = 0;
   float busvoltage = 0, current_mA = 0;

   // collect readings
   busvoltage = ina219.getBusVoltage_V();
   current_mA = ina219.getCurrent_mA();
   if (std::isnan(current_mA)) current_mA = 0.0f;
   Serial.printf("\n\nBusVolt = %.2f, curr(mA) = %.2f", busvoltage, current_mA);
   
   sht3x.requestReadings(); 
   sht3x.collectReadings();
   // bh17.requestReadings();
   // bh17.collectReadings();

   bh17.getReading();
   apds99.getReading();

   temp = sht3x.getTemp();
   hum = sht3x.getHum();
   lux1 = bh17.getLux();
   lux2 = apds99.getLux();

   Serial.printf("\ntemp = %.2f, hum = %.2f, lux1 = %.2f, lux2 = %.2f", temp, hum, lux1, lux2);

   // send readings
   // tweet.record.sendTempHumLux(temp, hum, lux, busvoltage, current_mA);
}

void configureESPNow() {
   wifi.setTxPower(0);
   wifi.startAP(true, BROADCAST_CHANNEL);
   tweet.setup(&espNow);
   espNow.setup(WiFi.channel());
   // Serial.print("Channel = "); Serial.println(WiFi.channel());
}

void setup() {
   // pinMode(PIN_DRIVE, OUTPUT);
   // digitalWrite(PIN_DRIVE, HIGH);            //! Power Lock
   Serial.begin(115200);
   // unsigned long timeRef = millis();

   // pinMode(PIN_CTRL1, INPUT);    //! Control Input1
   // pinMode(PIN_CTRL2, INPUT);    //! Control Input2
   // pinMode(PIN_MODE, INPUT_PULLUP);
   // pinMode(PIN_DONE, OUTPUT);
   pinMode(PIN_LED, OUTPUT);


   // bool read1 = digitalRead(PIN_CTRL1);
   // bool read2 = digitalRead(PIN_CTRL2);
   // configureESPNow();

   // if (read1 || read2) {
   //    read1 ? tweet.command.sendSingleClick(PIN_CTRL1) : tweet.command.sendSingleClick(PIN_CTRL2);
   //    delay(1);
   //    digitalWrite(PIN_DRIVE, LOW);             //! Power Release
   //    for (;;) {}

   // } else {
      // setup i2C
      Wire.begin(4, 5);
      sht3x.setup(&Wire);
      bh17.setup(&Wire);
      apds99.setup(&Wire);

      // Serial.print("TWEET MODE");
      if(!ina219.begin()){
         Serial.println("INA219 not connected!");
      }

      // request readings: need to wait at least 20ms before collect reading
      // start the Wifi to save wait time
   // }

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
   // if (currentMode == MODE_SETUP) {
   //    network.run();
   // } else {
      logSensors();
      digitalWrite(PIN_DONE, HIGH);
      digitalWrite(PIN_LED, !digitalRead(PIN_LED));
      delay(2000);
   // }

   // Serial.println("IM HERE AAA");
   // ESP.deepSleep(3000000, WAKE_RF_DEFAULT);
   // Serial.println("IM HERE BBBB");
}
