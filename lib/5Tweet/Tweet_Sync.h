class Tweet_Sync: public Interface_Tweet<SyncItem, CMD_SYNC> {
    public:
        std::function<void(uint8_t)> *onReceiveBounce;

        void sendSyncMock() {
            //! Master Initiate
            AppPrint("\n[TwSync]", __func__);
            item.cue = SYNC_MOCK;
            item.srcChannel = myChannel;
            item.timeStamp = millis();
            item.setSource(myMac);
            sendMessage2(55);
        }

        void handleMessage(SyncItem* receivedItem) {
            switch (receivedItem->cue) {
                case SYNC_MOCK: {
                    //! Slave received
                    AppPrint("\n[TwSync] send SYNC_BOUNCE");
                    memcpy(&item, receivedItem, sizeof(SyncItem));  // Clone Master message
                    AppPrint("ReceiveTime1", item.timeStamp);

                    item.destChannel = myChannel;
                    item.cue = SYNC_BOUNCE;
                    sendMessage2(55);
                    break;
                }
                case SYNC_BOUNCE: {
                    //! Master received
                    AppPrint("\n[TwSync] SYNC_BOUNCE received");
                    if (receivedItem->checkSource(myMac)) {
                        uint32_t transTime = receivedItem->getTransmitTime();
                        Serial.print("transTime = "); Serial.println(transTime);

                        if (onReceiveBounce) (*onReceiveBounce)(receivedItem->destChannel);
                    }
                    break;
                }
                default: {
                    AppPrint("[TwSync] Unknown");
                    break;
                }
            }
        }
};