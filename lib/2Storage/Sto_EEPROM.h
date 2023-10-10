#include <EEPROM.h>

// [0xDD, 0xDD, resetCnt_1, resetCnt_0 ....] 0-64
// [password] 65-140
// [name] 141-175
// [mqttServer] 176- 200
// [mqttTopic] 201-270

class Sto_EEPROM {
    protected:
        void writeBytes(int address, const void *value, size_t len) {
            byte* val = (byte*) value;

            for (int i=0; i<len; i++) {
                EEPROM.write(address+i, val[i]);
            }

            EEPROM.commit();
        }

        void readBytes(int address, void *value, size_t len) {
            byte* val = (byte*) value;

            for (int i=0; i<len; i++) {
                val[i] = EEPROM.read(address+i);
            }
        }

        void storeData(int address, const char *buf, size_t len) {
            for (int i=0; i<len; i++) {
                EEPROM.write(address+i, buf[i]);
            }

            EEPROM.commit();        
        }

        void writeByte(int address, uint8_t value) {
            EEPROM.write(address, value);
            EEPROM.commit(); 
        }

        uint8_t readByte(int address) {
            return EEPROM.read(address);
        }
};

class EEPROM_Check: public Sto_EEPROM {
    protected:
        int checkAddr;
        uint64_t valueAddr() { return checkAddr + 1; }

        void writeCode() {
            writeByte(checkAddr, 0xDD);
        }   

        bool checkCode() {
            return readByte(checkAddr) == 0xDD;
        }

        void clearCode() {
            writeByte(checkAddr, 0x00);
        }

    public:
        void loadCheckAddr(int addr) {
            checkAddr = addr;
        }
};

class EEPROM_ResetCount: public EEPROM_Check {
    void increaseValue() {
        readBytes(1, &value, sizeof(value));
        // value++;
        writeBytes(1, &value, sizeof(value));
    }

    public:
        uint64_t value = 0; 

        // Reset Code [0] = 0xDD, Reset Count [1-4]
        void loadValue() {
            loadCheckAddr(0);
            checkCode() ? increaseValue() : deleteValue();
        }

        void deleteValue() {
            AppPrint(__func__);
            writeCode();
            value = 0;
            writeBytes(1, &value, sizeof(value));
        }
};

class EEPROM_Data: public EEPROM_Check {
    public:
        bool loadData(void* data, size_t len) {
            // Serial.print("[EEPROM] "); AppPrint(__func__, "addr " + String(valueAddr()));
            if (!checkCode()) return false;
            readBytes(valueAddr(), data, len);
            return true;
        }

        void storeData(const void* data, size_t len) {
            // Serial.print("[EEPROM] "); AppPrint(__func__, "addr " + String(valueAddr()));
            writeBytes(valueAddr(), data, len);
            writeCode();
        }

        void deleteData() {
            clearCode();
        }
};

template<int startAddr, size_t len>
class EEPROM_FixData: public EEPROM_Data {
    public:
        EEPROM_FixData() {
            loadCheckAddr(startAddr);
        }

        void loadFixedData(void *data) {
            loadData(data, len);
        }

        void storeFixedData(const void *data) {
            storeData(data, len);
        }
};
