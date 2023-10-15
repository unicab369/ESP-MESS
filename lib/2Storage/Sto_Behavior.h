template <int arrAddress, class T, uint8_t count>
class Sto_Array: public Loggable {
   uint16_t getEEPROM_Address(uint8_t index) {
      //! offset by 2 = 1 for the checkByte + 1 for array element offset
      return arrAddress+index*(sizeof(T)+2);
   }

   protected:
      EEPROM_Value<T> rawData[count];
      bool isLoaded = false; 

      Sto_Array(const char* id): Loggable(id) {}
      
   public:
      T* getValueAt(uint8_t index) {
         return rawData[index].getValue();
      }

      void reload() {
         // xLogSectionf("%s count = %u", __func__, count);

         for (int i=0; i<count; i++) {
            uint16_t addr = getEEPROM_Address(i);
            rawData[i].loadData(addr);
         }

         // xLogSection("Print All Data\n");
         // AppPrintHex(rawData, 124);
         // isLoaded = true;
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
      return compare;
   }

   void assignPeerId(uint8_t id) {
      peerId = id;
   }

   void printRaw() {
      Serial.printf("Mac = %02X:%02X:%02X:%02X:%02X:%02X", 
                           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
      Serial.printf(" peerId = %u", peerId);
   }
};

#define MAX_PEER_COUNT 5

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
         uint8_t lastAvailIndex = INVALID_UINT8;

         PeerItem* match = firstMatch([&](PeerItem* item, uint8_t index) {
            bool foundMatch = item->hasSameMac(peerMac);

            if (foundMatch) {
               return true;
            } else if (!item->isValid()) {
               lastAvailIndex = index;
            }

            return false;
         });

         if (match != nullptr) {
            xLogf("foundIndex = %u", match->peerId);

         } else if (lastAvailIndex != INVALID_UINT8) {
            xLogf("**ADD NEW PEER at Index = %u", lastAvailIndex);
            PeerItem newPeer(peerMac);
            newPeer.assignPeerId(lastAvailIndex);
            newPeer.builtTime = 0x1122334455667788;
            updateData(lastAvailIndex, &newPeer);
         } else {
            
            xLogf("**NO AVAILABLE SPOT");
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
