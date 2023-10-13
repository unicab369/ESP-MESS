enum Cmd_Behavior: uint8_t {
    ACTION_OUTPUT = 0xA0,
    ACTION_WS2812 = 0xA1,
    ACTION_REPORT = 0xA2,
    ACTION_SEND = 0xA3,
    ACTION_NONE = 0xAF
};

enum Cue_Trigger: uint8_t {
    TRIGGER_STARTUP = 0xB0,
    TRIGGER_SINGLECLICK = 0xB1,
    TRIGGER_DOUBLECLICK = 0xB2,
    TRIGGER_PIR = 0xB3,
    TRIGGER_IR = 0xB4,
    TRIGGER_STATE = 0xB5,
};

struct BehaviorItem {
    Cue_Trigger cue;
    uint8_t refId;
    uint8_t data[32]; 
    
    void toCharArr(char* charArr) const {
        memcpy(charArr, this, sizeof(BehaviorItem));
    }

    template <typename T>
    void store(uint8_t refIdVal, Cue_Trigger cueVal, T control) {
        memcpy(data, &control, sizeof(T));
        refId = refIdVal;
        cue = cueVal;
    }

    template <typename T>
    void produce(T *control) {
        memcpy(control, data, sizeof(T));
    }
};

class ControlOutput {
    Cmd_Behavior actionCmd = ACTION_OUTPUT;

    public:
        uint8_t pin, value;

        ControlOutput(uint8_t pinVal, uint8_t valueVal) {
            pin = pinVal;
            value = valueVal;
        }

        bool extract(BehaviorItem* behav) {
            behav->produce(this);
            return actionCmd == ACTION_OUTPUT;
        }
};

class ControlWS2812 {
    Cmd_Behavior actionCmd = ACTION_WS2812;

    public:
        uint8_t pin, value;

        ControlWS2812(uint8_t pinVal, uint8_t valueVal) {
            pin = pinVal;
            value = valueVal;
        }

        bool extract(BehaviorItem* behav) {
            behav->produce(this);
            return actionCmd == ACTION_WS2812;
        }
};

class ControlReport {
    public:
        float value1, value2, value3;
};

class ControlSend {
    public:
        char myStr[16] = "testStr";
        Cmd_Behavior actionCmd = ACTION_SEND;
};

// class BehaviorItem {
//     public:
//         Cue_Trigger cue;
//         Cmd_Behavior actionCmd = ACTION_NONE;
//         uint8_t refId;
//         uint8_t data[32];

//         bool isValid() { return actionCmd != ACTION_NONE; }

//         void toCharArr(char* charArr) const {
//             memcpy(charArr, this, sizeof(BehaviorItem));
//         }

//         template <typename T>
//         void configure(uint8_t _refId, Cue_Trigger setCue, T control) {
//             memcpy(data, &control, sizeof(T));
//             cue = setCue;
//             refId = _refId;

//             if (std::is_same<T, ControlOutput>::value) {
//                 actionCmd = ACTION_OUTPUT;
//             } else if (std::is_same<T, ControlWS2812>::value) {
//                 actionCmd = ACTION_WS2812;
//             } else if (std::is_same<T, ControlSend>::value) {
//                 actionCmd = ACTION_SEND;
//             }
//         }

//         void handle(uint8_t _refId, Cue_Trigger checkCue) {
//             if (refId != _refId || cue != checkCue || !isValid()) return;
//             AppPrint("\n[Behav]", __func__);

//             switch (actionCmd) {
//                 case ACTION_OUTPUT: {
//                     ControlOutput control;
//                     control.load(data);

//                     // memcpy(&control, data, sizeof(control));
//                     AppPrint("ControlOutput Pin", String(control.pin));
//                     AppPrint("ControlOutput value", String(control.value));
//                     break;
//                 }
//                 case ACTION_WS2812: {
//                     ControlWS2812 control;
//                     control.load(data);

//                     // memcpy(&control, data, sizeof(control));
//                     AppPrint("ControlWS2812 pin", String(control.pin));
//                     AppPrint("ControlWS2812 value", String(control.value));
//                     break;
//                 }
//                 case ACTION_SEND: {
//                     ControlSend control;

//                     // memcpy(&control, data, sizeof(control));
//                     AppPrint("ControlWS2812 pin", String(control.myStr));
//                     break;
//                 }
//             }
//         }
// };

