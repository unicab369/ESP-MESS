class Mod_INA219: public SensorBase, public Interface_Current {
   void onReceiveData(uint16_t *buf) override {
      uint16_t val1 = *buf;
      uint16_t val2 = (val1 >> 3) * 4;
      setBusVoltage(val2/1000);
      Serial.printf("Volt Reading = %lu", val2);
   }

   public:
      //! addr 0x40
      Mod_INA219(): SensorBase(0x40), Interface_Current() {}

      uint16_t setup(TwoWire *wire) override {
         uint8_t cmd1[] = { 0x05 };
         uint16_t value = _setup(wire, cmd1, 2);

         uint32_t value2 = 4096;
         writeBuffer((byte*)value2, sizeof(value2));
         // return value;
         return 1;
      }

      bool requestReadings() override {
         uint8_t readCmd[] = { 0x02 };
         return _requestReadings(readCmd, 2, 2);
      }
};