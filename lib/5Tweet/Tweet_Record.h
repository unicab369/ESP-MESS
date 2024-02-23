class Tweet_Record: public Interface_Tweet<RecordItem, CMD_POST> {
    public:
        void sendTempHumLux(float temp, float hum, float lux, float volt, float mA) {
            AppPrint("\n[TweRecord]", __func__);
            item.update(temp, hum, lux, volt, mA);
            item.printData();
            sendMessage2();
        }
};