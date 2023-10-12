enum Cmd_Behavior: uint8_t {
    ACTION_OUTPUT,
    ACTION_WS2812,
    ACTION_REPORT,
    ACTION_SEND,
    ACTION_NONE
};

enum Cue_Trigger: uint8_t {
    TRIGGER_STARTUP,
    TRIGGER_SINGLECLICK,
    TRIGGER_DOUBLECLICK,
    TRIGGER_PIR,
    TRIGGER_IR,
    TRIGGER_STATE,
};

class Controlable {};

class ControlOutput: public Controlable {
    public:
        void load(uint8_t setPin, uint8_t setValue) {
            pin = setPin;
            value = setValue;
        }

        uint8_t pin;
        uint8_t value;
};

class ControlWS2812: public Controlable {
    public:
        void load(uint8_t setPin, uint32_t setValue) {
            pin = setPin;
            value = setValue;
        }

        uint8_t pin;
        uint32_t value;
};

class ControlReport: public Controlable {
    public:
        float value1, value2, value3;
};

class ControlSend: public Controlable {
    public:
        char myStr[16] = "testStr";
};

class BehaviorItem {
    public:
        BehaviorItem() {}

        Cue_Trigger cue;
        Cmd_Behavior actionCmd = ACTION_NONE;
        uint8_t refId;
        uint8_t data[32];

        bool isValid() { return actionCmd != ACTION_NONE; }

        void toCharArr(char* charArr) const {
            memcpy(charArr, this, sizeof(BehaviorItem));
        }

        template <typename T>
        void configure(Cue_Trigger setCue, T control) {
            memcpy(data, &control, sizeof(T));
            cue = setCue;
            
            if (std::is_same<T, ControlOutput>::value) {
                actionCmd = ACTION_OUTPUT;
            } else if (std::is_same<T, ControlWS2812>::value) {
                actionCmd = ACTION_WS2812;
            } else if (std::is_same<T, ControlSend>::value) {
                actionCmd = ACTION_SEND;
            }
        }

        void handle(Cue_Trigger checkCue) {
            if (cue != checkCue || isValid() == false) return;
            AppPrint("\n[Behav]", __func__);

            // (*callback)(&_control);
            switch (actionCmd) {
                case ACTION_OUTPUT: {
                    ControlOutput control;
                    memcpy(&control, data, sizeof(control));
                    AppPrint("ControlOutput Pin", String(control.pin));
                    AppPrint("ControlOutput value", String(control.value));
                    break;
                }
                case ACTION_WS2812: {
                    ControlWS2812 control;
                    memcpy(&control, data, sizeof(control));
                    AppPrint("ControlWS2812 pin", String(control.pin));
                    AppPrint("ControlWS2812 value", String(control.value));
                    break;
                }
                case ACTION_SEND: {
                    ControlSend control;
                    memcpy(&control, data, sizeof(control));
                    AppPrint("ControlWS2812 pin", String(control.myStr));
                    break;
                }
            }
        }
};

