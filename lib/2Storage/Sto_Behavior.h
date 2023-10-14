constexpr const char sto_arr_id[] = "Sto_Arr";
template <int address, class T, uint8_t count>
class Sto_Array: public Loggable {
   EEPROM_Value<T, sto_arr_id> rawData[count];

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
            xLog("***Reload index = %u", i);
            uint16_t addr = getEEPROM_Address(i);
            rawData[i].loadData(addr);
         }

         xLogSection("Print All Data\n");
         AppPrintHex(array, 48);
         isLoaded = true;
      }

      void deleteData() {
         for (int i=0; i<count; i++) {
            rawData[i].deleteData();
         }
         reload();   
      }

      void updateData(uint8_t index, T* newItem) {
         if (index>count) return;
         rawData[index].storeData();
         uint16_t addr = getEEPROM_Address(index);
         bool check2 = rawData[index].loadData(addr);
      }

      // T* firstMatch(std::function<bool(T*, uint8_t index)> cb) {
      //    if (!isLoaded) return NULL;
      //    T* output = NULL;

      //    for (int i=0; i<count; i++) {
      //       T* target = &array[i];
      //       bool match = cb(target, i);
      //       if (match) {
      //          output = target;
      //          break;
      //       }
      //    }

      //    return output;        
      // }

      void forEach(std::function<void(T*, uint8_t index)> cb) {
         if (!isLoaded) return;
         
         for (int i=0; i<count; i++) {
            cb(&array[i], i);
         }
      }
};

struct PeerItem {
   uint8_t mac[6];
   uint8_t peerId = INVALID_UINT8;  //! PeerId is assigned by Sto_Peer
   uint64_t builtTime = 0;

   PeerItem() {}
   
   PeerItem(const uint8_t macVal[6], uint64_t time = 0) {
      memcpy(mac, macVal, sizeof(mac));
      builtTime = time;
   }

   bool isValid() { return peerId != INVALID_UINT8; }

   bool hasSameMac(uint8_t* targetMac) {
      Serial.println("\n***ITEM "); printRaw(); Serial.println();
      Serial.printf("***TARGET_MAC=%02X:%02X:%02X:%02X:%02X:%02X", 
                           targetMac[0], targetMac[1], targetMac[2], 
                           targetMac[3], targetMac[4], targetMac[5]);
      Serial.println();

      bool compare = memcmp(mac, targetMac, sizeof(mac)) == 0;
      Serial.printf("Compare = %u", compare); Serial.println();
      return compare;
   }

   void assignPeerId(uint8_t id) {
      peerId = id;
   }

   void printRaw() {
      Serial.printf("Mac=%02X:%02X:%02X:%02X:%02X:%02X", 
                           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
      // Serial.printf("\npeerId = %u, builtTime = %llu", peerId, builtTime);
      Serial.printf(" peerId = %u", peerId);
   }
};

#define MAX_PEER_COUNT 3

class Sto_Peer: public Sto_Array<1000, PeerItem, MAX_PEER_COUNT> {
   public:
      Sto_Peer(): Sto_Array("Sto_Peer") {}
      
      void printAllPeers() {
         xLogSection("***********");
         forEach([&](PeerItem* item, uint8_t index) {
            xLogLine();
            item->printRaw();
         });
      }

      void addPeer(uint8_t* peerMac) {
         uint8_t lastAvailIndex = INVALID_UINT8;
         PeerItem* match = NULL;

         for (int i=0; i<MAX_PEER_COUNT; i++) {
            PeerItem* target = &array[i];
            // bool foundMatch = target->hasSameMac(peerMac);

            Serial.println("\n***ITEM "); target->printRaw(); Serial.println();
            Serial.printf("***PEER_MAC=%02X:%02X:%02X:%02X:%02X:%02X", 
                                 peerMac[0], peerMac[1], peerMac[2], 
                                 peerMac[3], peerMac[4], peerMac[5]);
            Serial.println();

            bool foundMatch = memcmp(peerMac, target->mac, sizeof(target->mac)) == 0;
            Serial.printf("Compare = %u", foundMatch); Serial.println();

            if (foundMatch) {
               xLog("IM HERE YYYYYYYYY");
               match = target; break;
            } else if (!target->isValid()) {
               lastAvailIndex = i;
            }
         }

         if (match != nullptr) {
            xLogLine(); xLogf("**FOUND MATCH");
            xLogLine(); xLogf("foundIndex = %u", match->peerId);

         } else if (lastAvailIndex != INVALID_UINT8) {
            //! match == NULL
            xLogLine(); xLogf("\n**ADD NEW PEER at Index = %u", lastAvailIndex);
            PeerItem newPeer(peerMac);
            newPeer.assignPeerId(lastAvailIndex);
            updateData(lastAvailIndex, &newPeer);
         } else {
            xLogLine(); xLogf("\n**NO AVAILABLE SPOT");
         }

         printAllPeers();
      }

      // uint8_t findPeer(uint8_t* targetMac) {
      //    forEach([&](PeerItem* item, uint8_t index) {
      //       if (item->hasSameMac(targetMac)) {
      //          return item->peerId;
      //       }
      //    });

      //    return INVALID_UINT8;
      // }
};

#define MAX_BEHAVIOR_ITEMS 10

class Sto_Behavior: public Sto_Array<500, BehaviorItem, MAX_BEHAVIOR_ITEMS> {
   public:
      Sto_Behavior(): Sto_Array("Sto_Behav") {}
      
      void handleCue(uint8_t peerId, Cue_Trigger cue) {
         forEach([&](BehaviorItem* item, uint8_t index) {
            // xLogf("At Index = %u", index);
            if (item->check(peerId, cue) == false) return;      
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
