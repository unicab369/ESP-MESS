template <int address, class T, uint8_t count>
class Sto_Array: public Loggable {
   EEPROM_Data rawData[count];

   uint16_t getEEPROM_Address(uint8_t index) {
      return address+index*sizeof(T);
   }

   protected:
      T* array = new T[count];
      bool isLoaded = false; 

      Sto_Array(const char* id): Loggable(id) {}
      
   public:
      void reload() {
         for (int i=0; i<count; i++) {
            uint16_t addr = getEEPROM_Address(i);
            rawData[i].loadAddress(addr);
            rawData[i].loadData(&array[i], sizeof(T));
         }
         isLoaded = true;
      }

      void deleteData() {
         for (int i=0; i<count; i++) {
               uint16_t addr = getEEPROM_Address(i);
               rawData[i].deleteData();
         }
         reload();   
      }

      void updateData(uint8_t index, T* newItem) {
         if (index-1>count) return;
         rawData[index].storeData(newItem, sizeof(T));
         bool check2 = rawData[index].loadData(&array[index], sizeof(T));
      }

      void forEach(std::function<void(T*, uint8_t index)> onCallback) {
         if (!isLoaded) return;
         
         for (int i=0; i<count; i++) {
            onCallback(&array[i], i);
         }
      }
};

struct PeerItem {
   uint8_t mac[6];
   uint8_t refId = INVALID_UINT8;
   uint64_t builtTime;

   PeerItem() {}
   
   PeerItem(const uint8_t macVal[6], uint64_t time, uint8_t id) {
      memcpy(mac, macVal, sizeof(mac));
      builtTime = time;
      refId = id;
   }

   bool isSamePeer(uint8_t* macVal) {
      return memcmp(&mac, macVal, sizeof(mac)) == 0;
   }

   void printRaw() {
      Serial.printf("\nMac=%02X:%02X:%02X:%02X:%02X:%02X", 
                           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
      Serial.printf("\nrefId = %u, builtTime = %llu", refId, builtTime);
   }
};

#define MAX_PEER_COUNT 20

class Sto_Peer: public Sto_Array<1000, PeerItem, MAX_PEER_COUNT> {
   public:
      Sto_Peer(): Sto_Array("Sto_Peer") {}

      uint8_t findPeer(uint8_t* targetMac) {
         forEach([&](PeerItem* item, uint8_t index) {
            if (item->isSamePeer(targetMac)) {
               return item->refId;
            }
         });

         return INVALID_UINT8;
      }
};

#define MAX_BEHAVIOR_ITEMS 10

class Sto_Behavior: public Sto_Array<500, BehaviorItem, MAX_BEHAVIOR_ITEMS> {
   public:
      Sto_Behavior(): Sto_Array("Sto_Behav") {}

      void handleCue(uint8_t refId, Cue_Trigger cue) {
         forEach([&](BehaviorItem* item, uint8_t index) {
            // xLogf("At Index = %u", index);
            if (item->check(refId, cue) == false) return;      
            ControlOutput out1(0, 0);
            ControlWS2812 out2(0, 0);

            if (out1.extract(item)) {
               xLogf("IM HERE1 pin = %u, value = %u", out1.pin, out1.value);
            } else if (out2.extract(item)) {
               xLogf("IM HERE2 pin = %u, value = %u", out2.pin, out2.value);
            }
         });
      }
};
