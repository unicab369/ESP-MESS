class AnalogWritable: public PinWritable {
    int refValue = 0;

    public:
        void pin_analogWrite(int value) {
            // refValue = value;
            #ifdef ESP32
                ledcWrite(get_pin(), value);
            #else
                analogWrite(get_pin(), value);
            #endif
        }

        void fadeToward(int newValue, int change) {
            if (refValue>newValue) {
                Serial.print("+refVal="); Serial.print(refValue);
                Serial.print(" newVal="); Serial.println(newValue);
                refValue += change;
                refValue = refValue > newValue ? newValue : refValue;
            } else if (newValue > refValue) {
                Serial.print("-refVal="); Serial.print(refValue);
                Serial.print(" newVal="); Serial.println(newValue);
                refValue -= change;
                refValue = refValue < newValue ? newValue : refValue;
            }
            // Serial.print("value="); Serial.println(refValue);
            pin_analogWrite(refValue);
        }
};

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

class PinSequence: public AnalogWritable {
    Cycle_Timer cycle;
    ValueTimeSequence seq;

    std::function<void()> callback = [this]() {
        if (!seq.checkTimeout()) { return; }
        pin_analogWrite(seq.getCurrentValue());
    };

    public:
        void reload(PairValues values, int runTime = 0) {
            seq.load(values);
            cycle.loadCb(&callback);
            cycle.start(100, runTime);
        }

        void stop() {
            cycle.stop();
        }
        
        void run() {
            cycle.run();
        } 
};

class OutputController: public PinSequence {
    int outputs2[2] = { 255, 0 };
    int duration[2] = { 100, 100 };

    public:
        void repeatPulses(int pulseLength = 100) {
            duration[0] = pulseLength;
            duration[1] = pulseLength;
            reload(PairValues::make(outputs2, duration, 2));
        }

        void toggle() {
            stop();
            toggle();
        }

        void setON() {
            stop();
            turnOn();
        }

        void setOFF() {
            stop();
            turnOff();
        }
};