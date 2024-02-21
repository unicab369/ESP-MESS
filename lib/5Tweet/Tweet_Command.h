class Tweet_Command: public Interface_Tweet<CommandItem, CMD_TRIGGER> {
    public:
        void sendStartup() {
            item.update(TRIGGER_STARTUP);
            sendMessage2();
        }

        void sendSingleClick(uint8_t pin) {
            item.update(TRIGGER_SINGLECLICK, pin);
            sendMessage2();
        }

        void sendDoubleClick(uint8_t pin) {
            item.update(TRIGGER_DOUBLECLICK, pin);
            sendMessage2();
        }

        void sendPir(uint8_t pin, bool state) {
            item.update(TRIGGER_PIR, pin, state);
            sendMessage2();
        }

        void sendIr(uint32_t irCode) {
            item.update(TRIGGER_IR, irCode);
            sendMessage2();
        }
};