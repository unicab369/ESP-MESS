
struct PairValues {
    uint8_t *value1;
    uint16_t *value2;
    uint16_t length;

    static PairValues make(uint8_t *value1, uint16_t *value2, uint16_t length) {
        PairValues output;
        output.value1 = value1;
        output.value2 = value2;
        output.length = length;
        return output;
    }
};

class ValueTimeSequence {
    IntCycle frame;
    PairValues values;
    TimerItem timer;
    uint16_t timeoutDur;

    void reloadTimer() {
        timeoutDur = values.value2[frame.currentValue()];
        timer.reset(); 
    }

    public:
        void load(PairValues _values) {
            values = _values;
            frame.reload(_values.length);
            reloadTimer();    
        }

        int getCurrentValue() {
            int val = frame.currentValue();
            // Serial.print("Val="); Serial.println(val);
            return values.value1[val];
        }

        bool checkTimeout() {
            bool check = timer.isTimeout(timeoutDur);

            if (check) {
                // Serial.print("timeout dur = "); Serial.println(timeoutDur);
                frame.step();
                reloadTimer();
            }
            return check;
        }
};

class PinSequence: public PWMWritable, public Cycle_Timer {
    ValueTimeSequence seq;
    uint8_t toggleValue = 0;

    std::function<void()> callback = [this]() {
        if (!seq.checkTimeout()) { return; }
        pwmWrite(seq.getCurrentValue());
    };

    public:
        void reload(PairValues values, int runTime = 0) {
            seq.load(values);
            loadCb(&callback);
            start(25, runTime);
        }

        void turnON() override {
            stop();
            if (!isValid()) { return; }
            toggleValue = 1;
            pwmWrite(255);
        }

        void turnOFF() override {
            stop();
            if (!isValid()) { return; }
            toggleValue = 0;
            pwmWrite(0);
        }

        void toggle() {
            stop();
            if (!isValid()) { return; }
            toggleValue = (toggleValue == 0) ? 255 : 0;
            pwmWrite(toggleValue);
        }
};

class PulseController: public PinSequence {
    uint8_t outputs4[4] = { 255, 0, 255, 0 };
    uint16_t durations4[4] = { 100, 100, 100, 1000 };

    public:
        void uniformPulse20ms() {
            uniformPulses(20);
        }

        void uniformPulse500ms() {
            uniformPulses(500);
        }

        void uniformPluse1000ms() {
            uniformPulses(1000);
        }

        void uniformPulses(uint16_t pulseLength = 100) {
            singlePulses(pulseLength, pulseLength);
        }

        void singlePulses(uint16_t pulseLength = 100, uint16_t pitchLength = 1000) {
            durations4[0] = pulseLength;
            durations4[1] = 0;
            durations4[2] = 0;
            durations4[3] = pitchLength;
            outputs4[2] = 0;
            reload(PairValues::make(outputs4, durations4, 4));
        }

        void doublePulses(uint16_t pulseLength = 100, uint16_t pitchLength = 1000) {
            durations4[0] = pulseLength;
            durations4[1] = pulseLength;
            durations4[2] = pulseLength;
            durations4[3] = pitchLength;
            reload(PairValues::make(outputs4, durations4, 4));
        }

        void handle(ControlOutput control) {
            switch (control.value) {
                case 0: turnOFF(); break;
                case 1: turnON(); break;
                case 2: toggle(); break;
                default: break;
            }
        }
};