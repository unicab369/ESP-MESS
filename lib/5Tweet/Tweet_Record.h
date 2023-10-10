class Tweet_Record: public Interface_Tweet<RecordItem, CMD_POST> {
    public:
        void sendTempHumLux(float temp, float hum, float lux) {
            AppPrint("[TweRecord]", __func__);
            item.update(temp, hum, lux);
            sendMessage2();
        }

        void handleMessage(RecordItem* item) {
            switch (item->cue) {
                case RECORD_TEMPHUMLUX: {
                    AppPrint("RECORD_TEMPHUMLUX", "T=" + String(item->value1) + 
                                                    "H= " + String(item->value2) +
                                                    "L= " + String(item->value3));
                    break;
                }
                default: {
                    AppPrint("Receiv Unkwnown");
                    break;
                }
            }
        }
};