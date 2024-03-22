class SensorBase {
    int address;
    int dataLen = 0;

    protected:
        TwoWire *thisWire;
        SensorBase(int _addr): address(_addr) {}

        uint16_t _setup(TwoWire* _wire, byte* cmd, byte cmdLen, int delayMs = 20) {
            thisWire = _wire;

            delay(delayMs);
            writeBuffer(cmd, cmdLen);
            thisWire->requestFrom(address, 1);
            return thisWire->read();
        }

        bool _requestReadings(byte* cmd, byte cmdLen, byte _dataLen) {
            if (checkConnection()==false) return false;
            writeBuffer(cmd, cmdLen);
            dataLen = _dataLen;
            return true;
        }

        virtual void onReceiveData(uint8_t* buf) {}

    public:
        virtual uint16_t setup(TwoWire *wire) { return 0; }
        virtual bool requestReadings() { return false; }
        
        bool checkConnection() {
            thisWire->beginTransmission(address);
            byte err = thisWire->endTransmission();
            return err == 0;
        }

        bool collectReadings() {
            if (checkConnection()==false) return false;
            thisWire->requestFrom(address, dataLen);
            uint8_t buf[dataLen] = { 0 };

            for (byte i=0; i<dataLen; i++) {
                buf[i] = thisWire->read();
            } 

            onReceiveData(buf);
            return true;
        }

        //! Read Write into buffer
        bool readBuffer(uint8_t* output, byte len) {
            byte readLen = thisWire->requestFrom(address, len);
            for (byte i=0; i<len; i++) output[i] = thisWire->read();
            return readLen == len;
        }

        bool writeBuffer(byte *cmd, byte len) {
            if (len==0) return false;
            thisWire->beginTransmission(address);
            for (byte i=0; i<len; i++) thisWire->write(cmd[i]);
            return thisWire->endTransmission() == 0;
        }

        //! Read Write Value
        bool readValue(uint8_t cmd, uint8_t &value) {
            bool check = writeValue(cmd);
            thisWire->requestFrom(address, 1);
            value = Wire.read();
            return check;
        }

        bool writeValue(byte cmd) {
            thisWire->beginTransmission(address);
            thisWire->write(cmd);
            return thisWire->endTransmission() == 0;
        }

        //! Read uint16_t
        uint16_t makeUint16(uint8_t highByte, uint8_t lowByte) {
            return ((uint16_t)highByte << 8) | lowByte;
        }

        bool readUint16(uint8_t* cmd, uint8_t len, uint16_t &value, bool reverted = false) {
            bool check1 = writeBuffer(cmd, len);
            uint8_t byte_val[2];
            bool check2 = readBuffer(byte_val, 2);
            value = reverted ? makeUint16(byte_val[1], byte_val[0]) : makeUint16(byte_val[0], byte_val[1]);
            return check1;
        }
};

class Interface_TempHum {
    float temp = -1;
    float hum = -1;

    public:
        void setTemp(float _temp) { temp = _temp; }
        void setHum(float _hum) { hum = _hum; }

        float getTemp() { return temp; }
        float getHum()  { return hum; }

        void resetReadings() {
            temp = -1;
            hum = -1;
        }
};

class Interface_Lux {
    float lux = -1;

    public:
        void setLux(float _lux) { lux = _lux; }
        float getLux() { return lux; }

        void resetReadings() {
            lux = -1;
        }
};

class Interface_Current {
    float busVoltage = -1;
    float mA = -1;

    public:
        void setBusVoltage(float value) { busVoltage = value; }
        void setCurrent(float value) { mA = value; }

        float getBusVoltage() { return busVoltage; }
        float getmA() { return mA; }

        void resetReadings() {
            busVoltage = -1;
            mA = -1;
        }
};