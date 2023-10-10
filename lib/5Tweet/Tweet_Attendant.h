// Each device has an ladderId that's uniquedly assigned.
// the Master holds the first valid ladderId, which is 1.
// The Master sends the MASTER_RECRUITING command to start the recuitment process
// by deciding who it will assign as the next_ladderId to.

// There are 2 types of Slaves:
// - unassigned_slave: has ladderId == 0. 
// - assigned_slave: has ladderId != 0.

// assigned_slave notifies the master with the ATTENDANT_CLAIM if the Master next_ladderId matches its ladderId.
// When the Master receives this command, it sends the ATTENDANT_ASSIGN to requests a confirmation 
// from the matching slave. When the Master receives the ATTENDANT_CONFIRM command back from the salve, the assignment is 
// completed and the master increaments the next_ladderId. Next, it starts the MASTER_RECRUITING and continue the recruitment.

// unassigned_slave sends the SLAVE_PROPOSE command to propose itself as the potential next_ladderId.
// if the Master hasn't receive any ATTENDANT_CLAIM for the next_ladderId it broadcasted. It will proceed to start
// assigning the id to the first unassigned_slave that it receives the ATTENDANT_CONFIRM from.
// The Master picks a slave's MacAddress and sends the ATTENDANT_ASSIGN command.

class Tweet_Attendant: public Interface_Tweet<AttedantItem, CMD_ATTENDANT> {
    void send(CMD_Attedant cue) {
        item.cue = cue;
        sendMessage2();
    }

    std::function<void()> *setLadderIdCallback;

    public:
        bool isMaster() { return ladderId == 1; }
        uint16_t ladderId = 0;

        void reconfigure(uint8_t *mac, std::function<void()> *setLadderIdCb) {
            setup(mac);
            setLadderIdCallback = setLadderIdCb;
        }

        void startAttendant() {
            AppPrint("[TwAttendant]", __func__);
            ladderId = 1;
            item.next_ladderId = 1;
            if (setLadderIdCallback) (*setLadderIdCallback)();
            pollAttendant();
        }

        void pollAttendant() {
            AppPrint("[TwAttendant]", __func__);
            item.recuitCommand();
            sendMessage2();
        }

    void handleMessage(ReceivePacket2* packet) {
        switch (item.cue) {
            case MASTER_RECRUITING:  {
                //! Slave receives MASTER_RECRUITING command
                AppPrint("\n[Mesh] MASTER_RECRUITING Received", String(item.next_ladderId));
                bool isMatched = item.next_ladderId == ladderId;

                if (ladderId == 0) {
                    //! unassigned_slave sends SLAVE_PROPOSE command
                    AppPrint("[Mesh] SLAVE_PROPOSE Send");
                    item.setTarget(myMac);
                    send(SLAVE_PROPOSE);
                } else if (isMatched) {
                    //! maching slave sends SLAVE_CLAIM command
                    AppPrint("[Mesh] SLAVE_CLAIM Send");
                    item.setTarget(myMac);
                    send(SLAVE_CLAIM);
                }

                break;
            }

            case SLAVE_CLAIM: {
                if (isMaster()) {
                    //! Master receives SLAVE_CLAIM command 
                    //! Master sends MASTER_ASSIGN to the slave device that sends the SLAVE_CLAIM command
                    AppPrint("[Mesh] SLAVE_CLAIM Received", String(item.next_ladderId));
                    if (item.checkTarget(packet->sourceB)) { 
                        AppPrint("[Mesh] MASTER_ASSIGN Send", packet->getSource());
                        send(MASTER_ASSIGN);
                    }
                } else {
                    //! Stop attempt to send SLAVE_PROPOSE
                }

                break;
            }
            case SLAVE_PROPOSE: {
                if (isMaster()) {
                    //! Master receives SLAVE_PROPOSE command
                    //! Master sends MASTER_ASSIGN to the first slave it receives the message from
                    AppPrint("[Mesh] SLAVE_PROPOSE Received", String(item.next_ladderId)); 
                    if (item.checkTarget(packet->sourceB)) {
                        AppPrint("[Mesh] MASTER_ASSIGN Send", packet->getSource());
                        send(MASTER_ASSIGN);
                    }
                } else {

                }
                break;
            }
            case MASTER_ASSIGN: {
                if (!isMaster()) {
                    //! Slave receives MASTER_ASSIGN command
                    AppPrint("[Mesh] MASTER_ASSIGN Received", String(item.next_ladderId));
                    if (item.checkTarget(myMac)) {
                        //! if the targetMac matches the current slaveMac, send SLAVE_CONFIRM command
                        String str = String(myMac[0]) + ":" + String(myMac[1]) + ":" + String(myMac[2])
                            + ":" + String(myMac[3])  + ":" + String(myMac[4]) + ":" + String(myMac[5]);
                        AppPrint("[Mesh] SLAVE_CONFIRM Send", str);
                        ladderId = item.next_ladderId;
                        if (setLadderIdCallback) (*setLadderIdCallback)();
                        send(SLAVE_CONFIRM);
                    } else if (ladderId == item.next_ladderId) {
                        //! reset ladderId if the Master is assigning someone else
                        ladderId = 0;
                    }
                }
                break;
            }
            case SLAVE_CONFIRM: {
                if (isMaster()) {
                    //! Master receives SLAVE_CONFIRM command
                    AppPrint("[Mesh] SLAVE_CONFIRM Received", packet->getSource());
                    AppPrint("next_ladderId", String(item.next_ladderId));
                    // if (next_ladderId != attItem.next_ladderId) { return; }
                    //! Master continue recruitment of the next_ladderId
                    pollAttendant();

                } else {
                    // Stop attempt to send SLAVE_PROPOSE
                }

                break;
            }
        }
    }
};