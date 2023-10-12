#include <EEPROM.h>

// [0xDD, 0xDD, resetCnt_1, resetCnt_0 ....] 0-64
// [password] 65-140
// [name] 141-175
// [mqttServer] 176- 200
// [mqttTopic] 201-270

class Sto_EEPROM: public Loggable {
    public:
        Sto_EEPROM(): Loggable("EEPROM") {}

    protected:
        void writeValue(int address, uint64_t value) {
            for (int i = 0; i < 8; i++) {
                EEPROM.write(address + i, (uint8_t)(value >> (i * 8)));
            }
            EEPROM.commit(); // Commit the changes to EEPROM            
        }

        void readValue(int address, uint64_t* value) {
            *value = 0;
            for (int i = 0; i < 8; i++) {
                *value |= ((uint64_t)EEPROM.read(address + i) << (i * 8));
            }            
        }

        void writeBytes(int address, const void *value, size_t len) {
            xLogLine();
            xLogf("-&&& %s @addr = %u", __func__, address);
            byte* val = (byte*) value;
        
            for (int i=0; i<len; i++) {
                EEPROM.write(address+i, val[i]);
            }
            EEPROM.commit();

            // xLogLine();
            // xLog("ReadBack");
            // byte data[len];
            // readBytes(address, data, len);
            // AppPrintHex(data, len);
        }

        void readBytes(int address, void *value, size_t len) {
            xLogLine();
            xLogf("-&&& %s @addr = %u", __func__, address);
            byte* val = (byte*) value;

            for (int i=0; i<len; i++) {
                val[i] = EEPROM.read(address+i);
            }

            // byte checkData[len];
            // memcpy(checkData, val, len);
            // xLog("ReadBack2");
            // AppPrintHex(checkData, len);
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
        uint16_t valueAddr() { return checkAddr + 1; }

        //! loadAddress begins with check byte
        //! contents of the data follow the check byte
        void loadAddress(int addr) {
            checkAddr = addr;
        }
};

class EEPROM_ResetCount: public EEPROM_Check {
    void increaseValue() {
        //! NOTE: Read the value into a separte variable before writing it back into memory
        readValue(1, &value);
        value++;
        writeValue(1, value);
    }

    public:
        uint64_t value = 0; 

        // Reset Code [0] = 0xDD, Reset Count [1-8]
        void loadValue() {
            loadAddress(0);
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
            writeCode();
            writeBytes(valueAddr(), data, len);
        }

        void deleteData() {
            clearCode();
        }
};

template<int startAddr, size_t len>
class EEPROM_FixData: public EEPROM_Data {
    public:
        EEPROM_FixData() {
            loadAddress(startAddr);
        }

        void loadFixedData(void *data) {
            loadData(data, len);
        }

        void storeFixedData(const void *data) {
            storeData(data, len);
        }
};
