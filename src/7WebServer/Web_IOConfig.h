struct IOParams {
    char ioConf1[65] = "NULL";
    char ioConf2[65] = "NULL";
    char ioConf3[65] = "NULL";
    char ioConf4[65] = "NULL";
    char ioConf5[65] = "NULL";
};

class Web_IOConfig: public Web_Base {
    IOParams params;

    void makeContent() override {
        startPage();
        startTable("I/Os Configs");
        // server->sendContent("<form action='/'>");

        makeNewRow();
        makeTextArea("txt1", String(params.ioConf1));
        makeTextArea("txt2", String(params.ioConf2));
        makeTextArea("txt3", String(params.ioConf3));
        makeTextArea("txt4", String(params.ioConf4));
        makeTextArea("txt5", String(params.ioConf5));

        makeNewRow();
        makeFormButton("SAVE", "sav_ioConf");
        // server->sendContent("</form>");

        makeReturnButton();
        stopTable();
        stopPage();
    }

    std::function<void()> onSaveIoConf = [&]() {
        Serial.println("IM CALLED zzz");
        // String ioConf1 = server->arg("txt1");
        // String ioConf2 = server->arg("txt2");
        // String ioConf3 = server->arg("txt3");
        // String ioConf4 = server->arg("txt4");
        // String ioConf5 = server->arg("txt5");

        // if (saveConfigCb != NULL) { saveConfigCb(&ioConf1, &ioConf2, &ioConf3, &ioConf4, &ioConf5); }
    };

    public:
        Web_IOConfig(): Web_Base("Web_IO") {}

        std::function<void(IOParams*)> getConfigCb;
        std::function<void(String*, String*, String*, String*, String*)> saveConfigCb;

        void begin(Serv_Network *network, WebServer *server) {
            load("/ioConf", network, server);

            server->on("/sav_ioConf", HTTP_POST, onSaveIoConf);
            // if (getConfigCb != NULL) { getConfigCb(&params); }
            // makeContent();
        }
};