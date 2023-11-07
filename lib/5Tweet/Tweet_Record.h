class Tweet_Record: public Interface_Tweet<RecordItem, CMD_POST> {
    public:
        void sendTempHumLux(float temp, float hum, float lux, float volt, float mA) {
            AppPrint("[TweRecord]", __func__);
            item.update(temp, hum, lux, volt, mA);
            sendMessage2();
        }

        void handleMessage(RecordItem* item) {
            switch (item->cue) {
                case RECORD_TEMPHUMLUX: {
                    AppPrint("\nRECORD_TEMPHUMLUX", "T = " + String(item->value1) + 
                        ", H = " + String(item->value2) + ", L = " + String(item->value3) +
                        ", V = " + String(item->value4) + ", mA = " + String(item->value5));
                    break;
                }
                default: {
                    AppPrint("Receiv Unkwnown");
                    break;
                }
            }
        }
};