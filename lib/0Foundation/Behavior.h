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
        uint8_t pin;
        uint8_t value;

        void load(uint8_t setPin, uint8_t setValue) {
            pin = setPin;
            value = setValue;
        }

        void load(uint8_t *data) {
            memcpy(this, data, sizeof(*this));
        }
};

class ControlWS2812: public Controlable {
    public:
        uint8_t pin;
        uint32_t value;

        void load(uint8_t setPin, uint32_t setValue) {
            pin = setPin;
            value = setValue;
        }

        void load(uint8_t *data) {
            memcpy(this, data, sizeof(*this));
        }
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
        void configure(uint8_t _refId, Cue_Trigger setCue, T control) {
            memcpy(data, &control, sizeof(T));
            cue = setCue;
            refId = _refId;

            if (std::is_same<T, ControlOutput>::value) {
                actionCmd = ACTION_OUTPUT;
            } else if (std::is_same<T, ControlWS2812>::value) {
                actionCmd = ACTION_WS2812;
            } else if (std::is_same<T, ControlSend>::value) {
                actionCmd = ACTION_SEND;
            }
        }

        void handle(uint8_t _refId, Cue_Trigger checkCue) {
            if (refId != _refId || cue != checkCue || !isValid()) return;
            AppPrint("\n[Behav]", __func__);

            switch (actionCmd) {
                case ACTION_OUTPUT: {
                    ControlOutput control;
                    control.load(data);

                    // memcpy(&control, data, sizeof(control));
                    AppPrint("ControlOutput Pin", String(control.pin));
                    AppPrint("ControlOutput value", String(control.value));
                    break;
                }
                case ACTION_WS2812: {
                    ControlWS2812 control;
                    control.load(data);

                    // memcpy(&control, data, sizeof(control));
                    AppPrint("ControlWS2812 pin", String(control.pin));
                    AppPrint("ControlWS2812 value", String(control.value));
                    break;
                }
                case ACTION_SEND: {
                    ControlSend control;

                    // memcpy(&control, data, sizeof(control));
                    AppPrint("ControlWS2812 pin", String(control.myStr));
                    break;
                }
            }
        }
};

