class Mod_INA219: public SensorBase, public Current_Interface {
   void onReceiveData(uint16_t *buf) override {
      uint16_t val1 = *buf;
      uint16_t val2 = (val1 >> 3) * 4;
      setBusVoltage(val2/1000);
      Serial.printf("Volt Reading = %lu", val2);
      // Serial.printf("value2 = %lu", buf);
      // Serial.printf("value3 = %04X", buf);
      // float val = getBusVoltage();
      // Serial.printf("Volt Reading2 = %lu", val);

      // float value = (float)((buf>>3)*4)/1000.0f;
      // setBusVoltage(value);

      // int32_t temp = (int32_t)(((uint32_t)buf[0] << 8) | buf[1]);
      // temp = ((4375 * temp) >> 14) - 4500;
      // float tempValue = (float)temp / 100.0f;
      // setTemp(tempValue);

      // uint32_t hum = ((uint32_t)buf[3] << 8) | buf[4];
      // hum = (625 * hum) >> 12;
      // float humValue = (float)hum / 100.0f;
      // setHum(humValue);
   }

   public:
      //! addr 0x40
      Mod_INA219(): SensorBase(0x40), Current_Interface() {}

      uint16_t setup(TwoWire *wire) override {
         uint8_t cmd1[] = { 0x05 };
         uint16_t value = _setup(wire, cmd1, 2);

         uint32_t value2 = 4096;
         registerCmd((byte*)value2, sizeof(value2));
         // return value;
         return 1;
      }

      bool requestReadings() override {
         uint8_t readCmd[] = { 0x02 };
         return _requestReadings(readCmd, 2, 15, 2);
      }
};