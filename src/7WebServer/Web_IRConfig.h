class ParamsObj {
    public:
        static const int MAX_PARAMS = 10;
        char params[ParamsObj::MAX_PARAMS][208] = {{}};

        void forEachParam(std::function<void(int, char*)> loadParam) {
            for (int i=0; i<ParamsObj::MAX_PARAMS; i++) {
                loadParam(i, params[i]);
            }
        }

        void clear() {
            memset(params, 0, sizeof(params));
        }
};

class Web_IRConfig: public Web_Base {
    void makeContent() override {
        if (getConfigCb3 != NULL) { 
            // paramsObj.clear();
            // getConfigCb3(&paramsObj);
        }

        startPage();
        startTable("IR Configs");
        // server->sendContent("<form action='/'>");

        makeNewRow();
        // paramsObj.forEachParam([](int index, char *param) {
        //     sendTextArea("txt" + String(index), String(param));
        // });

        makeNewRow();
        makeFormButton("SAVE", "sav_irConf");
        // server->sendContent("</form>");

        makeReturnButton();
        stopTable();
        stopPage();
    }

    public:
        Web_IRConfig(): Web_Base("Web_IR") {}

        std::function<void(ParamsObj*)> getConfigCb3;
        std::function<void(ParamsObj*)> saveConfigCb3;

        void begin(Serv_Network *network, WebServer *server) {
            load("/irConf", network, server);

            server->on("/sav_irConf", [&]() {
                Serial.println("IM CALLED yyy");
                // paramsObj.forEachParam([](int index, char *param) {
                //     strcpy(param, server->arg("txt" + String(index)).c_str());
                // });

                // if (saveConfigCb3 != NULL) { saveConfigCb3(&paramsObj); }
            });
        }
};

