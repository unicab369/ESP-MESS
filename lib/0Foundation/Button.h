enum BTN_Action {
    ACTION_SINGLE_CLICK,
    ACTION_DOUBLE_CLICK,
    ACTION_PRESS_ACTIVE,
    ACTION_PRESS_END
};

enum BNT_State {
    BUTTON_PULSE_LO,
    BUTTON_PULSE_HI,
    BUTTON_PULSE_LO2,
    BUTTON_PULSE_HI2,
    BUTTON_HOLD_BEGIN,
    BUTTON_END
};

enum BNT_Hold {
    HOLD_5_SEC,
    HOLD_10_SEC,
    HOLD_15_SEC,
    HOLD_20_SEC,
    HOLD_25_SEC,
    HOLD_TRANSITION
};

class MyButton: private PinReadable, public Loggable {
    BNT_State state = BUTTON_END;
    TimeoutItem debounce_timer;
    TimeoutItem click_timer;
    TimeoutItem press_timer;
    BNT_Hold holdStatus = HOLD_TRANSITION;

    void reset_timers(BNT_State newState) {
        debounce_timer.reset();
        click_timer.reset();
        state = newState;

        if (state == BUTTON_PULSE_HI || state == BUTTON_PULSE_HI2) {
            press_timer.reset();
        }
    }

    void Handle_ButtonEnd(BTN_Action action, uint32_t elapse) {
        (*callback)(action, HOLD_TRANSITION, elapse);
        state = BUTTON_END;          
    }

    BNT_Hold currentHoldStatus() {
        uint32_t elapse = press_timer.elapsed();
        if (elapse > 20000) { return HOLD_20_SEC; } 
        else if (elapse > 10000) { return HOLD_10_SEC; }
        else if (elapse > 5000) { return HOLD_5_SEC; } 
        else { return HOLD_TRANSITION; }
    }

    public:
        std::function<void(BTN_Action, BNT_Hold, uint32_t)> *callback;

        MyButton(): Loggable("Bnt") {}

        void setup(uint8_t pin, uint32_t debounceTicks = 50, 
                            uint32_t clickTicks = 200, uint32_t longPressTicks = 2000) {
            pin_setup(pin, true);
            if (!isValid()) { return; }
            debounce_timer.load(debounceTicks);
            click_timer.load(clickTicks-debounceTicks);
            press_timer.load(longPressTicks);
        }

        void run() {
            if (callback == nullptr) { return; }
            bool read = !pin_read();

            if (read && state == BUTTON_END) {
                reset_timers(BUTTON_PULSE_HI);
            } 
            else if (state == BUTTON_PULSE_HI && debounce_timer.check()) {
                if (read) {
                    // ButtonHold Reset
                    if (press_timer.check()) { reset_timers(BUTTON_HOLD_BEGIN); }
                } else {
                    reset_timers(BUTTON_PULSE_LO);
                }
            } 
            else if (state == BUTTON_PULSE_LO && debounce_timer.check()) {
                if (!read) {
                    //! Single Click
                    if (click_timer.check()) {
                        xLogLine("SingleClick");
                        Handle_ButtonEnd(ACTION_SINGLE_CLICK, 0);
                    }
                } else {
                    reset_timers(BUTTON_PULSE_HI2);
                }
            } 
            else if (state == BUTTON_PULSE_HI2 && debounce_timer.check()) {
                if (read) {
                    // ButtonHold Reset
                    if (press_timer.check()) { reset_timers(BUTTON_HOLD_BEGIN); }
                } else {
                    reset_timers(BUTTON_PULSE_LO2);
                }
            } 
            else if (state == BUTTON_PULSE_LO2 && debounce_timer.check()) {
                if (read) {
                    // ButtonHold Reset
                    if (press_timer.check()) { reset_timers(BUTTON_HOLD_BEGIN); }
                } else {
                    //! Double Click
                    if (click_timer.check()) {
                        xLogLine("DoubleClick");
                        Handle_ButtonEnd(ACTION_DOUBLE_CLICK, 0);
                    }
                }
            } 
            else if (state == BUTTON_HOLD_BEGIN) {
                uint32_t elapse = press_timer.elapsed();

                if (read) {
                    if (press_timer.check()) {

                        //! Press Active
                        // only return holdStatus change once
                        BNT_Hold _holdOutput = currentHoldStatus();
                        if (holdStatus == _holdOutput) {
                            _holdOutput = HOLD_TRANSITION;
                        } else {
                            holdStatus = _holdOutput;
                        }
                        
                        // DO NOT call Handle_ButtonEnd
                        (*callback)(ACTION_PRESS_ACTIVE, _holdOutput, elapse);
                    }
                } else {
                    //! Press Ended
                    xLogLinef("PressEnded elapse = %llu", elapse);
                    Handle_ButtonEnd(ACTION_PRESS_END, elapse);
                }
            } 
        }
};
