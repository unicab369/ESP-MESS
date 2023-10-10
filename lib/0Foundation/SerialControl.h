#include <sstream>

class SerialControl {
    uint8_t position = 0;
    char inputString[100] = "";
    
    public:
        std::function<void(const char *ssid, const char *password)> *onStoreCred;

        void run() {
            if (!Serial.available() || onStoreCred == nullptr) return;

            while (Serial.available()) {
                char c = Serial.read(); 

                if (c=='\b' && position>=0) {
                    inputString[position] = 0;
                    if (position != 0) { position--; }
                } else if (c == '\n') {
                    // Pointer to point the word.
                    // format: cred *ssid* *passw*
                    char *ref = strtok(inputString, " ");
                    if (strcmp(ref, "cred") == 0) {
                        char ssid[33] = { 0 }, passw[64] = { 0 };
                        ref = strtok(NULL, " ");
                        strcpy(ssid, ref);
                        ref = strtok(NULL, " ");
                        ref[strlen(ref)-1] = '\0';      //! replace \n with string terminator
                        strcpy(passw, ref);
                        (*onStoreCred)(ssid, passw);
                    }
                    
                    memset(inputString, 0, 100);
                    position = 0;
                } else if (position < 100) {
                    inputString[position] = c;
                    position++;
                }
            }
            
            Serial.print("cmd: "); Serial.println(inputString);
        }
};