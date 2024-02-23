#define LOG_MAX_LENGTH 128

class Loggable {
    char identifier[20];
    
    public:
        Loggable(const char* id) {
            memcpy(identifier, id, sizeof(identifier));
        }

        void xLog(const char* value, const char* separator = "") {
            Serial.printf("[%s]%s %s\n", identifier, separator, value);
        }

        void xLog(const char* value, uint64_t number) {
            //# TODO: There seem to be an issue with xLog when printing a large uint64_t number
            //! Need to investigate
            xLogf("%s %llu", value, number);
        }

        void xLogf(const char *format, ...) {
            char buf1[LOG_MAX_LENGTH]; 
            va_list args;

            va_start(args, format);
            vsnprintf(buf1, sizeof(buf1), format, args);
            va_end(args);

            xLog(buf1);
        }

        void xLogLine(const char* value, const char* separator = ":") {
            Serial.println();
            xLog(value, separator);
        }

        void xLogLinef(const char *format, ...) {
            char buf1[LOG_MAX_LENGTH]; 
            va_list args;

            va_start(args, format);
            vsnprintf(buf1, sizeof(buf1), format, args);
            va_end(args);

            xLogLine(buf1);
        }

        void xLogSection(const char* value) {
            Serial.println();
            xLogf("%s %s", "###########", value);
        }

        void xLogSectionf(const char *format, ...) {
            char buf1[LOG_MAX_LENGTH]; 
            va_list args;

            va_start(args, format);
            vsnprintf(buf1, sizeof(buf1), format, args);
            va_end(args);

            xLogSection(buf1);
        }

        void xLogStatus(const char* value, bool check) {
            xLogf("%s %s", value, check ? "OK" : "Failed!");
        }
}; 