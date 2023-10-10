enum Direction {
    DIRECTION_FORWARD,
    DIRECTION_REVERSE,
    DIRECTION_EXPAND
};

class IntCycle {
    int value = 0;
    int limit = 0;

    public:
        IntCycle() {}

        void reload(int _limit, int startValue = 0) {
            limit =_limit;
            if (startValue <= limit) { value = startValue; } 
        }

        int currentValue()  { return value; }
        int getLimit()      { return limit; }
        bool isZero()       { return value == 0; }

        bool isEndOfCycle() { 
            // Serial.print("value="); Serial.print(value); Serial.print(" limit="); Serial.println(limit);
            return value == limit-1; 
        }

        int nextValue(int step = 1) {
            int newValue = value+step;
            if (newValue >= limit) { newValue = newValue-limit; }
            else if (newValue < 0) { newValue = limit + step; }
            return newValue;
        }

        int step(int step = 1) {
            value = nextValue(step);
            return value;
        }

        void reset() {
            value = 0;
        }
};

class SequenceFader: public IntCycle {
    public:
        int valueForIndex(int index) {
            int dif = index - currentValue();
            int refIndex = dif%getLimit();
            if (dif < 0) { refIndex = getLimit() + dif; }

            // Serial.print("*pixelIndex="); Serial.print(index);
            // Serial.print(" / "); Serial.print(frameIndex.currentValue());
            // Serial.print(" => "); Serial.println(refIndex);
            return refIndex;
        }

        int valueForRevertIndex(int index) {
            int output = getLimit() - valueForIndex(index);
            return output;
        }
};