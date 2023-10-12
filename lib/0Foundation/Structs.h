struct MinMax {
    uint16_t min;
    uint16_t max;

    MinMax make(uint16_t min, uint16_t max) {
        MinMax output;
        output.min = min;
        output.max = max;
        return output;
    }
};