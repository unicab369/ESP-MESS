typedef std::function<void(float, float, float, float, float)> TweetRecordCb;

class Tweet_Record: public Interface_Tweet<RecordItem, CMD_POST> {
    public:
        TweetRecordCb *callback;

        void sendTempHumLux(float temp, float hum, float lux, float volt, float mA) {
            AppPrint("\n[TweRecord]", __func__);
            item.update(temp, hum, lux, volt, mA);
            sendMessage2();
        }
};