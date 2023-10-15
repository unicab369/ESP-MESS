template <int address, class T, uint8_t count>
class Sto_Array: public Loggable {
   uint16_t getEEPROM_Address(uint8_t index) {
      return address+index*sizeof(T);
   }

   protected:
      EEPROM_Value<T> rawData[count];
      // T* array = new T[count];
      bool isLoaded = false; 

      Sto_Array(const char* id): Loggable(id) {}
      
   public:
      T* getValueAt(uint8_t index) {
         return rawData[index].getValue();
      }

      void reload() {
         xLogSection(__func__);

         for (int i=0; i<count; i++) {
            xLog("***Reload index = %u", i);
            uint16_t addr = getEEPROM_Address(i);
            rawData[i].loadData(addr);
         }

         xLogSection("Print All Data\n");
         AppPrintHex(rawData, 48);
         isLoaded = true;
      }

      void deleteData() {
         for (int i=0; i<count; i++) {
            rawData[i].deleteData();
         }
         reload();   
      }

      void updateData(uint8_t index, T* newItem) {
         if (index>=count) return;
         rawData[index].updateData(newItem);
         reload();
      }

      void forEach(std::function<void(T*, uint8_t index)> cb) {
         if (!isLoaded) return;
         
         for (int i=0; i<count; i++) {
            cb(getValueAt(i), i);
         }
      }

      T* firstMatch(std::function<bool(T*, uint8_t index)> cb) {
         if (!isLoaded) return NULL;

         for (int i=0; i<count; i++) {
            T* target = getValueAt(i);
            bool match = cb(target, i);
            if (match) return target;
         }

         return NULL;
      }

};

//! This object get stored in EEPROM
//! please keep size minimal, dont inherit Loggable
struct PeerItem {
   uint8_t mac[6];
   uint8_t peerId = INVALID_UINT8;
   uint64_t builtTime = 0;

   PeerItem() {}
   
   PeerItem(const uint8_t macVal[6], uint64_t time = 0) {
      memcpy(mac, macVal, sizeof(mac));
      builtTime = time;
   }

   //! isValid if peerId is not 255 && mac is not 00:00:00:00:00:00
   bool isValid() {
      uint8_t macZero[6] = {};
      bool isMacZero = memcmp(mac, macZero, sizeof(mac)) == 0;
      return peerId != INVALID_UINT8 && !isMacZero;
   }

   bool hasSameMac(uint8_t* targetMac) {
      bool compare = memcmp(mac, targetMac, sizeof(mac)) == 0;
      // Serial.printf("Compare = %u", compare); Serial.println();
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
         xLogSection(__func__);
         forEach([&](PeerItem* item, uint8_t index) {
            item->printRaw();
            Serial.println();
         });
      }

      void addPeer(uint8_t* peerMac) {
         xLogf("%s %02X:%02X:%02X:%02X:%02X:%02X", __func__,
                     peerMac[0], peerMac[1], peerMac[2], 
                     peerMac[3], peerMac[4], peerMac[5]);
         Serial.println();

         uint8_t lastAvailIndex = INVALID_UINT8;

         // PeerItem* match = firstMatch([&](PeerItem* item, uint8_t index) {
         //    Serial.print("***Try Add = "); item->printRaw(); Serial.println();
         //    bool foundMatch = item->hasSameMac(peerMac);

         //    if (foundMatch) {
         //       xLogLine("**Found MATCH");
         //       return true;
         //    } else if (!item->isValid()) {
         //       lastAvailIndex = index;
         //       xLogLinef("**Update LasAvailIndex = %u", lastAvailIndex);
         //    }

         //    return false;
         // });

         PeerItem* match = NULL;
         for (int i=0; i<MAX_PEER_COUNT; i++) {
            PeerItem* target = getValueAt(i);
            Serial.print("***Try Add = "); target->printRaw(); Serial.println();
            bool foundMatch = target->hasSameMac(peerMac);

            if (foundMatch) {
               xLogLine("**Found MATCH");
               match = target; break;
            } else if (!target->isValid()) {
               lastAvailIndex = i;
               xLogLinef("**Update LasAvailIndex = %u", lastAvailIndex);
            }
         }

         if (match != nullptr) {
            xLogLinef("foundIndex = %u", match->peerId);

         } else if (lastAvailIndex != INVALID_UINT8) {
            //! match == NULL
            xLogLinef("**ADD NEW PEER at Index = %u", lastAvailIndex);
            PeerItem newPeer(peerMac);
            newPeer.assignPeerId(lastAvailIndex);
            updateData(lastAvailIndex, &newPeer);
         } else {
            xLogLinef("**NO AVAILABLE SPOT");
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
