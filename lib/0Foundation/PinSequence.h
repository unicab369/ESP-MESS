
struct PairValues {
    int *value1;
    int *value2;
    int length;

    static PairValues make(int *value1, int *value2, int length) {
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
    TimeoutItem timeout;

    void reloadTimeout() {
        timeout.load(values.value2[frame.currentValue()]);
    }

    public:
        void load(PairValues _values) {
            values = _values;
            frame.reload(_values.length);
            reloadTimeout();      
        }

        int getCurrentValue() {
            int val = frame.currentValue();
            // Serial.print("Val="); Serial.println(val);
            return values.value1[val];
        }

        bool checkTimeout() {
            bool check = timeout.check();
            if (check) {
                frame.step();
                reloadTimeout();
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
            start(100, runTime);
        }

        void turnON() override {
            stop();
            if (!isValid()) { return; }
            pwmWrite(255);
        }

        void turnOFF() override {
            stop();
            if (!isValid()) { return; }
            pwmWrite(0);
        }

        void toggle() {
            stop();
            if (!isValid()) { return; }
            toggleValue = (toggleValue == 0) ? 255 : 0;
            pwmWrite(toggleValue);
        }
};

class SinglePulse: public PinSequence {
    int outputs[2] = { 255, 0 };
    int durations[2] = { 100, 100 };

    public:
        void repeatPulses(int pulseLength = 100) {
            durations[0] = pulseLength;
            durations[1] = pulseLength;
            reload(PairValues::make(outputs, durations, 2));
        }
};