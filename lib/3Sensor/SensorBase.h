class SensorBase {
    int address;
    int dataLen = 0;

    protected:
        TwoWire *thisWire;
        SensorBase(int _addr): address(_addr) {}

        uint16_t _setup(TwoWire* _wire, byte* cmd, byte cmdLen, int delayMs = 20) {
            thisWire = _wire;
            registerCmd(cmd, cmdLen);
            delay(delayMs);
            thisWire->requestFrom(address, 1);
            return thisWire->read();
        }

        bool _requestReadings(byte* cmd, byte cmdLen, unsigned long wait, byte _dataLen) {
            if (checkConnection()==false) return false;
            registerCmd(cmd, cmdLen);
            dataLen = _dataLen;
            return true;
        }

        virtual void onReceiveData(uint16_t* buf) {}

    public:
        virtual uint16_t setup(TwoWire *wire) { return 0; }
        virtual bool requestReadings() { return false; }
        virtual void reset() { }
        
        bool checkConnection() {
            thisWire->beginTransmission(address);
            byte err = thisWire->endTransmission();
            return err == 0;
        }

        void registerCmd(byte *cmd, byte len) {
            if (len==0) { return; }
            thisWire->beginTransmission(address);
            for (byte i=0; i<len; i++) { thisWire->write(cmd[i]); }
            thisWire->endTransmission();
        }

        bool collectReadings() {
            if (checkConnection()==false) return false;
            thisWire->requestFrom(address, dataLen);
            uint16_t buf[dataLen] = { 0 };

            for (byte i=0; i<dataLen; i++) {
                buf[i] = thisWire->read();
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

class Current_Interface {
    float busVoltage = -1;
    float mA = -1;

    public:
        void setBusVoltage(float value) { busVoltage = value; }
        void setCurrent(float value) { mA = value; }

        float getBusVoltage() { return busVoltage; }
        float getmA() { return mA; }

        void reset() {
            busVoltage = -1;
            mA = -1;
        }
};