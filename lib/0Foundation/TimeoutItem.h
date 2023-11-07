//! AtomicOnce
class AtomicOnce {
    bool didRun = false;

    public:
        AtomicOnce() {}

        void atomic_reset() {
            didRun = false;
        }

        bool atomic_didRun() {
            if (!didRun) {
                didRun = true;
                return false;  // return false for first time
            }

            return true;
        }
};

//! TimerItem
class TimerItem {
    uint32_t timeRef = 0;

    public:
        void reset() {
            timeRef = millis();
        }

        uint32_t elapsed() {
            return millis() - timeRef;
        }

        bool isTimeout(uint32_t timeOffset) {
            return elapsed() > timeOffset;
        }

        void logElapse(const char* label = "start") {
            AppPrint('\n' + label, elapsed());
            reset();
        }

        //# This function will limit the callback to only be called on a certain duration. 
        //# It does it by Reseting the timer when it's Timeout and only Then performs the Callback
        void onTimeoutWithReset(uint32_t duration, std::function<void()> onComplete) {
            if (!isTimeout(duration)) return;
            onComplete();
            reset();
        }
};

//! TimeoutItem
class TimeoutItem {
    TimerItem timer;
    uint32_t cooldown = 0;
    bool _isActive = false;

    public:
        bool isZero() { return cooldown == 0; }
        uint32_t elapsed(bool resetOnElapsed = false) { 
            uint32_t output = timer.elapsed();
            if (resetOnElapsed) reset();
            return output; 
        }
        
        void load(uint32_t _cooldown) {
            cooldown = _cooldown;
            reset();
        }

        void reset() {
            timer.reset();
            _isActive = true;
        }

        bool check(bool resetOnTimeout = false) {
            if (!_isActive || isZero()) return false;
            bool timeout = timer.isTimeout(cooldown);
            if (timeout && resetOnTimeout) reset();
            return timeout;
        }        
        
        bool update(uint32_t duration) {
            if (!_isActive) return false;
            bool timeout = timer.isTimeout(duration);
            if (timeout) reset();
            return timeout;            
        }
};


//! IntSequence
class IntSequence {
    unsigned int *sequence;
    unsigned int target_value = 0;
    int target_index = 0;
    int len = 0;

    public: 
        IntSequence() {}

        // load - the sequence and it's length
        void sequence_load(unsigned int *_sequence, int _len) {
            sequence = _sequence;
            len = _len;
        }

        unsigned int sequence_getValue() {
            if (sequence == NULL || sequence_check()) { return 0; }
            target_value = sequence[target_index];
            // Serial.print("\t-Seq["); Serial.print(target_index); 
            // Serial.print("] = "); Serial.println(target_value);
            return target_value;
        }

        void sequence_reset() {
            target_index = 0;
        }

        bool sequence_check() {
            bool complete = target_index+1 > len;
            // Serial.print("\t-Seq = " + String(target_index+1) + " | " + String(len));
            // Serial.print("\t-SeqComplete = "); Serial.println(complete);
            return complete;
        }

        void sequence_next() {
            target_index++;
        }
};


//! Cycle_Timer
class Cycle_Timer {
    TimeoutItem frameTimer;
    TimeoutItem durationTimer;
    
    bool _isActive = false;
    std::function<void()>* executeCb;
    std::function<void()>* completionCb;

    public:
        void loadCb(std::function<void()>* execute, std::function<void()>* completion = NULL) {
            executeCb = execute;        // call on each frame
            completionCb = completion;  // call when cycle completed
        }

        void start(int frameDur, int duration) {
            durationTimer.load(duration);
            frameTimer.load(frameDur);
            _isActive = true;
            if (*executeCb) { (*executeCb)(); }
        }

        void stop() {
            _isActive = false;
        } 

        void run() {
            if (!_isActive || !frameTimer.check(true)) return;
            if (durationTimer.check(true)) {
                if (*completionCb) { (*completionCb)(); }
                _isActive = false;
                return;
            }

            if (*executeCb) { (*executeCb)(); }
        }
};
