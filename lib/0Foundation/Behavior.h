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
   uint8_t peerId;
   uint8_t data[32]; 
   
   void toCharArr(char* charArr) const {
      memcpy(charArr, this, sizeof(BehaviorItem));
   }

   bool check(uint8_t peerIdIdVal, Cue_Trigger cueVal) {
      return peerId == peerIdIdVal && cue == cueVal;
   }
   
   template <typename T>
   void load(uint8_t peerIdIdVal, Cue_Trigger cueVal, T* control) {
      memcpy(data, control, sizeof(T));
      peerId = peerIdIdVal;
      cue = cueVal;
   }

   template <typename T>
   void produce(T *control) {
      memcpy(control, data, sizeof(T));
   }

   void printRaw() {
      Serial.printf("cue = %u, peerId = %u", cue, peerId);
      AppPrintHex(data, sizeof(data));
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

