#define MAX_BEHAVIOR_ITEMS 10

class Sto_Behavior: public Loggable {
   EEPROM_Data rawData[MAX_BEHAVIOR_ITEMS];
   BehaviorItem* behaviors;
   bool isLoaded = false; 

   //! getAddress
   uint16_t getAddress(uint8_t index) {
      return 500+index*sizeof(BehaviorItem);
   }

   void reload() {
      for (int i=0; i<MAX_BEHAVIOR_ITEMS; i++) {
         uint16_t addr = getAddress(i);
         rawData[i].loadAddress(addr);
         rawData[i].loadData(&behaviors[i], sizeof(BehaviorItem));
      }
   }

   public:
      Sto_Behavior(): Loggable("Sto_Behav") {}

      void setup() {
         behaviors = new BehaviorItem[MAX_BEHAVIOR_ITEMS];
         reload();
         isLoaded = true;
      }

      void deleteData() {
         for (int i=0; i<MAX_BEHAVIOR_ITEMS; i++) {
               uint16_t addr = getAddress(i);
               rawData[i].deleteData();
         }
         reload();          
      }

      void updateData(uint8_t index, BehaviorItem* newItem) {
         if (index-1>MAX_BEHAVIOR_ITEMS) return;
         rawData[index].storeData(newItem, sizeof(BehaviorItem));
         bool check2 = rawData[index].loadData(&behaviors[index], sizeof(BehaviorItem));
         // xLogf("RetrievedData2 cue = 0x%02X; cmd = 0x%02X", behaviors[index].cue, behaviors[index].actionCmd);
      }

      void handleCue(uint8_t refId, Cue_Trigger cue) {
         if (!isLoaded) return;

         for (int i=0; i<MAX_BEHAVIOR_ITEMS; i++) {
            BehaviorItem *behav = &behaviors[i];
            if (behav->cue != cue) continue;
            
            ControlOutput out1(0, 0);
            ControlWS2812 out2(0, 0);

            if (out1.extract(behav)) {
               xLogf("IM HERE1 pin = %u, value = %u", out1.pin, out1.value);
            } else if (out2.extract(behav)) {
               xLogf("IM HERE2 pin = %u, value = %u", out2.pin, out2.value);
            }
               // behaviors[i].handle(refId, cue);
         }
      }
};
