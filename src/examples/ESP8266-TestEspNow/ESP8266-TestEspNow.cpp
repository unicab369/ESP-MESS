// pio run -e ESP8266-TestEspNow -t upload --upload-port COM4

#include <Arduino.h>
#include <functional>
#include <Wire.h>
#include <SPI.h>

#include <0Foundation.h>
#include "3Mng_Runtime.h"
// #include <Adafruit_INA219.h>
#include <INA219_WE.h>

INA219_WE ina219c = INA219_WE(0x40);

#define PIN_CTRL1 12
#define PIN_CTRL2 14
#define PIN_DONE 13
#define PIN_DRIVE 15
#define PIN_LED 2

enum DeviceMode {
   MODE_LOWPOWER,
   MODE_TRIGGER,
   MODE_SETUP
};

Mod2_SHT3 sht3x;
Mod2_BH17 bh17;
Serv_Tweet tweet;
Serv_EspNow espNow;
Net_Wifi wifi;
TimeoutItem testTimer;       //* print time logger
int BROADCAST_CHANNEL = 6;

Serv_Device device;
Mng_Network network;
DeviceMode currentMode = MODE_LOWPOWER;
unsigned long timeRef = millis();
// Adafruit_INA219 ina219;

Mod_APDS9930 apds99;
Mod_INA219 ina219b;

void configureESPNow() {
   wifi.setTxPower(0);
   wifi.startAP(true, BROADCAST_CHANNEL);
   tweet.setup(&espNow);
   espNow.setup(WiFi.channel());
}

void setup() {
   pinMode(PIN_DRIVE, OUTPUT);
   digitalWrite(PIN_DRIVE, HIGH);      //! POWER LOCK

   Serial.begin(115200);
   pinMode(PIN_CTRL1, INPUT);          //! Control Input1
   pinMode(PIN_CTRL2, INPUT);          //! Control Input2
   pinMode(PIN_DONE, OUTPUT);
   pinMode(PIN_LED, OUTPUT);
   configureESPNow();

   //! check mode
   bool read1 = digitalRead(PIN_CTRL1);
   bool read2 = digitalRead(PIN_CTRL2);

   if (read1 && !read2 || !read1 && read2) {
      currentMode = MODE_TRIGGER;
   } 

   if (read1 && read2) {
      Serial.print("SETUP MODE");
      device.configure();
      network.setup(&device);
      currentMode = MODE_SETUP;

      for (;;) {
         if (millis() - timeRef > 100) {
            digitalWrite(PIN_LED, !digitalRead(PIN_LED));
            timeRef = millis();
         }
      }

   } else if (read1 || read2) {
      read1 ? tweet.command.sendSingleClick(PIN_CTRL1) : tweet.command.sendSingleClick(PIN_CTRL2);
      delay(1);
      digitalWrite(PIN_DRIVE, LOW);             //! POWER RELEASE

      for (;;) {
         if (millis() - timeRef > 500) {
            digitalWrite(PIN_LED, !digitalRead(PIN_LED));
            timeRef = millis();
         }

         // Serial.println("IM HERE AAA");
         // ESP.deepSleep(6000000);
         // Serial.println("IM HERE BBBB");
      }

   } else {
      // setup i2C
      Wire.begin(4, 5);
      sht3x.setup(&Wire);
      bh17.setup(&Wire);
      apds99.setup(&Wire);

      if(!ina219c.init()){
         Serial.println("INA219 not connected!");
      }
      
      uint16_t confRead = ina219c.readRegister(INA219_WE::INA219_CONF_REG);
      Serial.printf("\nINA29_CONFIG_REG = 0x%04X", confRead);

      uint16 calRead = ina219c.readRegister(INA219_WE::INA219_CAL_REG);
      Serial.printf("\nINA29_CAL_REG = 0x%04X", calRead);

      ina219b.setup(&Wire);

      // request readings: need to wait at least 20ms before collect reading
      // start the Wifi to save wait time
   }
   // Serial.printf("\nTImeDIf = %lu", millis()-timeRef);
}

void logSensors() {
   float temp = 0, hum = 0, lux1 = 0, lux2 = 0;
   float busvoltage = 0, current_mA = 0;

   // // collect readings
   // busvoltage = ina219.getBusVoltage_V();
   // current_mA = ina219.getCurrent_mA();
   // if (std::isnan(current_mA)) current_mA = 0.0f;
   // Serial.printf("\n\nBusVolt = %.2f, curr(mA) = %.2f", busvoltage, current_mA);
   
   // ina219b.getReading();

   // sht3x.requestReadings(); 
   // sht3x.collectReadings();
   // bh17.requestReadings();
   // bh17.collectReadings();

   sht3x.getReading();
   bh17.getReading();
   apds99.getReading();
   ina219b.getReading();

   busvoltage = ina219c.getBusVoltage_V();
   current_mA = ina219c.getCurrent_mA();
   Serial.printf("\n\nbusVoltage = %.2f, mA= %.2f", busvoltage, current_mA);

   busvoltage = ina219b.getBusVoltage();
   current_mA = ina219b.getmA();

   temp = sht3x.getTemp();
   hum = sht3x.getHum();
   lux1 = bh17.getLux();
   lux2 = apds99.getLux();

   // send readings
   Serial.printf("\ntemp = %.2f, hum = %.2f, lux1 = %.2f, lux2 = %.2f", temp, hum, lux1, lux2);
   Serial.printf("\n\n***busVoltage = %.2f, mA= %.2f", busvoltage, current_mA);
   // tweet.record.sendTempHumLux(temp, hum, lux1, busvoltage, current_mA);
}


void loop() {
   if (currentMode == MODE_LOWPOWER) {
      logSensors();
      digitalWrite(PIN_DRIVE, LOW);             //! POWER RELEASE
      digitalWrite(PIN_DONE, HIGH);             //! TIMER RELEASE
      digitalWrite(PIN_LED, !digitalRead(PIN_LED));
      delay(2000);
   } 
}
