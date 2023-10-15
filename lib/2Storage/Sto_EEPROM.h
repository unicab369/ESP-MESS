#include <EEPROM.h>

// [0xDD, 0xDD, resetCnt_1, resetCnt_0 ....] 0-64
// [password] 65-140
// [name] 141-175
// [mqttServer] 176- 200
// [mqttTopic] 201-270

class Sto_EEPROM {
    protected:
        Loggable logger = Loggable("EEPROM");

        void writeValue(uint16_t address, uint64_t value) {
            for (int i = 0; i < 8; i++) {
                EEPROM.write(address + i, (uint8_t)(value >> (i * 8)));
            }
            EEPROM.commit(); // Commit the changes to EEPROM            
        }

        void readValue(uint16_t address, uint64_t* value) {
            *value = 0;
            for (int i = 0; i < 8; i++) {
                *value |= ((uint64_t)EEPROM.read(address + i) << (i * 8));
            }            
        }

        void deleteBytes(uint16_t address, uint8_t value, size_t len) {
            logger.xLogf("&&& %s @addr = %u", __func__, address);
            for (int i=0; i<len; i++) {
                EEPROM.write(address+i, value);
            }
            EEPROM.commit();

            // xLogLine(); xLog("ReadBack");
            // byte data[len];
            // readBytes(address, data, len);
            // AppPrintHex(data, len);
        }

        void writeBytes(uint16_t address, const void *value, size_t len) {
            logger.xLogf("&&& %s @addr = %u", __func__, address);
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

        void readBytes(uint16_t address, void *value, size_t len) {
            logger.xLogf("&&& %s @addr = %u len = %zu", __func__, address, len);
            byte* val = (byte*) value;

            for (int i=0; i<len; i++) {
                val[i] = EEPROM.read(address+i);
            }

            // byte checkData[len];
            // memcpy(checkData, val, len);
            // xLog("ReadBack2");
            // AppPrintHex(checkData, len);
        }

        void storeData(uint16_t address, const char *buf, size_t len) {
            for (int i=0; i<len; i++) {
                EEPROM.write(address+i, buf[i]);
            }

            EEPROM.commit();        
        }

        void writeByte(uint16_t address, uint8_t value) {
            EEPROM.write(address, value);
            EEPROM.commit(); 
        }

        uint8_t readByte(uint16_t address) {
            return EEPROM.read(address);
        }
};


template <class T>
class EEPROM_Value: public Sto_EEPROM {
    protected:
        void writeCode() {
            writeByte(startAddr, 0xDD);
        }   

        bool checkCode() {
            return readByte(startAddr) == 0xDD;
        }

        void clearCode() {
            writeByte(startAddr, 0x00);
        }

        //! startAddr contains checkByte, content follows startAddr
        uint16_t startAddr = 0;
        uint16_t contentAddr() { return startAddr + 1; }
        T value;

        //! interface
        virtual bool makeExtraction(char *input) { return false; }
        virtual void print() {}

    public:
        //! interface
        bool extractor(char *input) {
            bool check = makeExtraction(input);
            if (check) {
                Serial.println("IM HERE UUUUUU");
                storeData();
                reloadData();
            }

            return check;
        }
        

        bool loadData(uint16_t addr) {
            startAddr = addr;
            if (!checkCode()) return false;
            readBytes(contentAddr(), &value, sizeof(T));
            print();
            return true;
        }

        void reloadData() {
            loadData(startAddr);
        }

        void updateData(T* newValue) {
            writeCode();
            writeBytes(contentAddr(), newValue, sizeof(T));
        }

        void storeData() {
            writeCode();
            writeBytes(contentAddr(), &value, sizeof(T));
        }

        void deleteData() {
            writeCode();
            deleteBytes(contentAddr(), 0, sizeof(T));
        }
};