class SensorBase {
    TwoWire *wire;
    int address;
    int dataLen = 0;

    void registerCmd(byte *cmd, byte len) {
        if (len==0) { return; }
        wire->beginTransmission(address);
        for (byte i=0; i<len; i++) { wire->write(cmd[i]); }
        wire->endTransmission();
    }

    protected:
        SensorBase(int _addr): address(_addr) {}

        uint16_t _setup(TwoWire* _wire, byte* cmd, byte cmdLen) {
            wire = _wire;
            registerCmd(cmd, cmdLen);
            delay(20);
            wire->requestFrom(address, 1);
            return wire->read();
        }

        bool _requestReadings(byte* cmd, byte cmdLen, unsigned long wait, byte _dataLen) {
            wire->beginTransmission(address);
            if(wire->endTransmission() != 0) return false;

            registerCmd(cmd, cmdLen);
            dataLen = _dataLen;
            return true;
        }

        virtual void onReceiveData(uint16_t* buf) {}

    public:
        virtual uint16_t setup(TwoWire *wire) { return 0; }
        virtual bool requestReadings() { return false; }
        virtual void reset() { }
        
        bool collectReadings() {
            wire->requestFrom(address, dataLen);
            uint16_t buf[dataLen] = { 0 };

            for (byte i=0; i<dataLen; i++) {
                buf[i] = wire->read();
            } 

            onReceiveData(buf);
            return true;
        }
};

class TempHum_Interface {
    float temp = -1;
    float hum = -1;

    public:
        void setTemp(float _temp) { temp = _temp; }
        void setHum(float _hum) { hum = _hum; }

        float getTemp() { return temp; }
        float getHum()  { return hum; }
};

class Lux_Interface {
    float lux = -1;

    public:
        void setLux(float _lux) { lux = _lux; }
        float getLux() { return lux; }
};