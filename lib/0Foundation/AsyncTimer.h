#define SERVICE_BEGIN_TIME 3

struct RunTimeModel {
    uint32_t maxLoopTime = 0;

    public:
        void updateMaxLoopTime(uint32_t startTime = 0) {
            uint32_t loopTime = millis() - startTime;
            if (loopTime > maxLoopTime) maxLoopTime = loopTime;
        }

        //! NOTE: Not for overwriting outside of AsyncTimer
        uint8_t seconds = 0, minutes = 0; 
        uint32_t hours = 0;
        uint32_t cycleCounter = 0;

        uint16_t millisec = 0;  
        bool secondsChanged = false, minutesChanged = false, hoursChanged = false;
        bool isEven = false;

        bool isSecondInterval(int value) { return seconds%value==0; }
        bool isHundredMsInterval(int value) { return millisec%value==0; }
        
        void resetFlags() {
            secondsChanged = false;
            minutesChanged = false;
            hoursChanged = false;
            cycleCounter = 0;
            maxLoopTime = 0;
        }

        void updateSeconds() {
            millisec = 0;
            isEven = !isEven;
            secondsChanged = true;
            seconds++;

            if (seconds >= 60) {
                // update Minutes
                seconds = 0;
                minutesChanged = true;
                minutes++;
                
                if (minutes >= 60) {
                    // update Hours
                    minutes = 0;
                    hoursChanged = true;
                    hours++;
                }
            }
        }

        void countCycle() {
            cycleCounter++;
        }

        String getRecords() {
            char output[22];
            sprintf(output, "%02d:%02d:%02d %u ~%u", hours, minutes, seconds, 
                                                    cycleCounter, maxLoopTime);
            return String(output);
        }
};


#ifdef ESP32
    #include "soc/timer_group_struct.h"
    #include "soc/timer_group_reg.h"

    //! Timer Group 0
    hw_timer_t *timer0 = NULL;
    volatile bool interruptFlag0 = false;
    portMUX_TYPE timer0_Mux = portMUX_INITIALIZER_UNLOCKED;

    void IRAM_ATTR onTimer0() {
        portENTER_CRITICAL(&timer0_Mux);
        interruptFlag0 = true;
        portEXIT_CRITICAL(&timer0_Mux);
    }

    void setupInterrupts() {
        timer0 = timerBegin(0, 80, true);               // prescaler 80 (1MHz), count up
        timerAttachInterrupt(timer0, &onTimer0, true); 
        timerAlarmWrite(timer0, 25000, true);         // 1 second (1,000,000 microseconds)
        timerAlarmEnable(timer0);
    }
#endif

class AsyncTimer {
    uint32_t timeRef = 0;
    std::function<void(RunTimeModel*)>* callback;
    uint8_t coreId;
    uint16_t lastRead = 0;

    public:
        uint32_t stackUsage = 0;
        RunTimeModel model;
        
        String record() { return model.getRecords(); }

        void setup(std::function<void(RunTimeModel*)> *cb) {
            // timeRef = millis();
            callback = cb;
        }
        
        void run() {
            model.countCycle();     // get Reset every second
            // if (callback == nullptr) return;
            // uint32_t startTime = millis();
            // uint32_t timeDif = startTime - timeRef;

            if (model.millisec == lastRead || callback == nullptr) return;
            // if (timeDif < model.millisec + 25) return;
            // model.millisec +=25;

            uint32_t startTime = millis();
            
            if (model.millisec >= 1000) {                
                #ifdef ESP32
                    stackUsage = uxTaskGetStackHighWaterMark(NULL);
                    // Serial.print("Core# = "); Serial.println(xPortGetCoreID());
                    // feed the WDT to prevent crash
                    // if (xPortGetCoreID() == 0) {
                    //     TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
                    //     TIMERG0.wdt_feed=1;
                    //     TIMERG0.wdt_wprotect=0;
                    // } else if (xPortGetCoreID() == 1) {
                    //     TIMERG1.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
                    //     TIMERG1.wdt_feed=1;
                    //     TIMERG1.wdt_wprotect=0;     
                    // }
                #endif
    
                model.updateSeconds();      // ORDER DOES MATTER: update seconds before processCb
                (*callback)(&model);        // ORDER DOES MATTER: Pinning!
                model.resetFlags();         // ORDER DOES MATTER: reset flags after processCb 
                timeRef = millis();      // ORDER DOES MATTER: reset timeRef if seconds changed
            } else {
                (*callback)(&model);
            }

            model.updateMaxLoopTime(startTime);
            lastRead = model.millisec;
        }   
};