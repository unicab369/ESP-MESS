byte SHT_RESET_CMD[2]           = { 0x30, 0xA2 } ;
byte SHT_HIGHREP_HOLD_CMD[2]    = { 0x2C, 0x06 };
byte SHT_MEDREP_HOLD_CMD[2]     = { 0x2C, 0x0D };
byte SHT_LOWREP_HOLD_CMD[2]     = { 0x2C, 0x10 };
byte SHT_HIGHREP_FREE_CMD[2]    = { 0x24, 0x00 };
byte SHT_MEDREP_FREE_CMD[2]     = { 0x24, 0x0B };
byte SHT_LOWREP_FREE_CMD[2]     = { 0x24, 0x16 };

class Mod2_SHT3: public SensorBase, public Interface_TempHum {
    void onReceiveData(uint16_t *buf) override {
        int32_t temp = (int32_t)(((uint32_t)buf[0] << 8) | buf[1]);
        temp = ((4375 * temp) >> 14) - 4500;
        float tempValue = (float)temp / 100.0f;
        setTemp(tempValue);

        uint32_t hum = ((uint32_t)buf[3] << 8) | buf[4];
        hum = (625 * hum) >> 12;
        float humValue = (float)hum / 100.0f;
        setHum(humValue);
    }

    public:
        //! addr 0x44
        Mod2_SHT3(): SensorBase(0x44), Interface_TempHum() {}

        uint16_t setup(TwoWire *wire) override {
            uint16_t value = _setup(wire, SHT_RESET_CMD, 2, 0);
            return value;
        }

        bool requestReadings() override {
            return _requestReadings(SHT_HIGHREP_FREE_CMD, 2, 6);
        }
};