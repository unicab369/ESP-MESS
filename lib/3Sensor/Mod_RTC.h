#include "RTClib.h"

class Mod_RTC: public I2CInterface, public Loggable {
    RTC_DS1307 rtc;
    bool didLoad = false;
    char dateTimeStr[22];
    char dateStr[11];

    //! addr 0x68
    public:
        Mod_RTC(): I2CInterface(), Loggable("RTC") {}

        DateTime getTimeNow() { return getTime(); }
        bool isRunning() { return rtc.isrunning(); }
        
        void setup(TwoWire *wire = &Wire) {
            bool check = rtc.begin(wire);
            begin(wire, 0x68);
            xLogStatus(__func__, check);
            if (!check) return; 
            // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            didLoad = true;
        }

        void update(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                    uint8_t min, uint8_t sec) {
            if (!didLoad) { return; }
            DateTime date = DateTime(year, month, day, hour, min, sec);
            rtc.adjust(date);
        }

        void configure(uint32_t value) {
            DateTime dateTime = DateTime(value);
            rtc.adjust(dateTime);
        }

        DateTime getTime() { return rtc.now(); }

        // String timeDisplay() {
        //     if (!didLoad) { return "<null>"; }
        //     char output[22];
        //     DateTime time = getTime();
        //     sprintf(output, "%02d:%02d:%02d %02d:%02d:%02d", time.hour(), 
        //             time.minute(), time.second(), time.month(), time.day(), time.year());
        //     return String(output);
        // }

        char* getDateTimeStr() {
            DateTime time = getTime();
            snprintf_P(dateTimeStr, sizeof(dateTimeStr), PSTR("%02u:%02u:%02u %02u-%02u-%02u"), 
                        time.hour(), time.minute(), time.second(), time.month(), time.day(), time.year());
            return dateTimeStr;             
        }
};

class Mng_AppClock: public ClockItem {
    Mod_RTC *rtc;
    bool isLoaded = false;

    public:
        void setup(Mod_RTC *_rtc) {
            rtc = _rtc;
            begin();
            syncTimers();
            isLoaded = true;
        }

        void syncTimers() {
            if (!isLoaded) return;

            AppPrintHeap();
            AppPrint("[AppClock] Clock_Time", getDateTimeStr());
            AppPrint("[AppClock] RTC_Time", rtc->getDateTimeStr());

            if (!rtc->checkDevice()) return;
            uint32_t rtcUnixTime = rtc->getTimeNow().unixtime();

            // if (rtcUnixTime > getTimeNow()) {
            //     AppPrint("[AppClock]", "Sync with RTC = " + String(rtcUnixTime));
            //     configure(rtcUnixTime, TIME_RTC);
            // }
        }

        void updateTimers(time_t time) {
            if (!isLoaded) return;
            AppPrint("[AppClock]", __func__);

            if (rtc->checkDevice()) {
                rtc->configure(time);
                configure(time, TIME_RTC);
            } else {
                configure(time, TIME_NETWORK);
            }
        }
};