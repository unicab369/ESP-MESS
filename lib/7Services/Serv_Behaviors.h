class Serv_Behavior {
	public:
		// Sto_Peer stoPeer;
		// Sto_Behavior stoBehav;
		
		template <Cue_Trigger trigger, class T>
		void storeAction(uint8_t behavIndex, T* action, uint8_t* peerMac) {
			// uint8_t id = stoPeer.addPeer(peerMac);


			Data_Behavior behav_In;
			behav_In.load(0x99, trigger, action);
			// behav_In.load(peer->peerId, trigger, action);
			// stoBehav.updateData(behavIndex, &behav_In);     //! store behavior
		}

		void handleCommandItem(uint8_t* source, CommandItem *item) {
			// uint8_t match = stoPeer.findPeer(source);
			// if (match != INVALID_UINT8) {
			//     stoBehav.handleCue(match, item->cue);
			// }
		}

		void test() {
			// stoBehav.handleCue(0x01, TRIGGER_SINGLECLICK);
			// stoBehav.handleCue(0x01, TRIGGER_DOUBLECLICK);
		}
};