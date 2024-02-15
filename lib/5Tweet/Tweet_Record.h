typedef std::function<void(float, float, float, float, float)> TweetRecordCb;

class Tweet_Record: public Interface_Tweet<RecordItem, CMD_POST> {
    public:
        TweetRecordCb *callback;

        void sendTempHumLux(float temp, float hum, float lux, float volt, float mA) {
            AppPrint("\n[TweRecord]", __func__);
            item.update(temp, hum, lux, volt, mA);
            sendMessage2();
        }

        void handleMessage(RecordItem* item) {
            switch (item->cue) {
                case RECORD_TEMPHUMLUX: {
                    AppPrint("\nRECORD_TEMPHUMLUX", "T=" + String(item->value1) + 
                        ", H=" + String(item->value2) + ", L=" + String(item->value3) +
                        ", V=" + String(item->value4) + ", mA=" + String(item->value5));
                    //     if (callback != nullptr) (*callback)(item->value1, item->value2, item->value3, item->value4, item->value5);
                    break;
                }
                default: {
                    AppPrint("Receiv Unkwnown");
                    break;
                }
            }
        }
};