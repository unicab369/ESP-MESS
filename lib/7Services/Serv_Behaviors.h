class Serv_Behavior {
    Sto_Behavior stoBehav;
    Sto_Peer stoPeer;

    public:
        void setup() {
            stoBehav.reload();
            stoPeer.reload();
        }

        template <Cue_Trigger trigger, class T>
        void storeAction(uint8_t behavIndex, T* action, PeerItem* peer) {
            // BehaviorItem behav_In;
            // behav_In.load(peer->peerId, trigger, action);
            // stoBehav.updateData(behavIndex, &behav_In);     //! store behavior
            // stoPeer.updateData(peer->peerId, peer);          //! store peer
            stoPeer.addPeer(peer);
        }

        void handleCommandItem(uint8_t* source, CommandItem *item) {
            uint8_t match = stoPeer.findPeer(source);
            if (match != INVALID_UINT8) {
                stoBehav.handleCue(match, item->cue);
            }
        }

        void test() {
            stoBehav.handleCue(0x01, TRIGGER_SINGLECLICK);
            stoBehav.handleCue(0x01, TRIGGER_DOUBLECLICK);

            // stoBehavior->deleteData();
            // stoBehavior->storeData();
        }
};