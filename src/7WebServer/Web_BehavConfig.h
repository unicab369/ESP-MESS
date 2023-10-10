class Web_BehavConfig: public Web_Base {
    void makeContent() override {
        // if (getConfigCb3 != NULL) { 
        //     // paramsObj.clear();
        //     // getConfigCb3(&paramsObj);
        // }

        startPage();
        startTable("Behaviors Configs");
        // server->sendContent("<form action='/'>");

        makeNewRow();
        // paramsObj.forEachParam([this](int index, char *param) {
        //     makeTextArea("txt" + String(index), String(param));
        // });

        makeNewRow();

        makeFormButton("SAVE", "sav_behavConf");
        // server->sendContent("</form>");

        makeReturnButton();
        stopTable();
        stopPage();
    }

    public:
        Web_BehavConfig(): Web_Base("Web_Behav") {}
        
        void begin(Serv_Network *network, WebServer *server) {
            load("/behavConf", network, server);

            server->on("/sav_behavConf", [&]() {
                // paramsObj.forEachParam([this](int index, char *param) {
                //     strcpy(param, server->arg("txt" + String(index)).c_str());
                // });

                // if (saveConfigCb3 != NULL) { saveConfigCb3(&paramsObj); }
            });
        }
};