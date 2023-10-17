class Web_DevConfig: public Web_Base {
    char ssid[33] = "";
    char password[64] = "";
    char deviceName[21] = "";
    char mqttServer[21] = "";

    void makeContent() override {
        startPage();
        startTable("Device Configs");
        // server->sendContent("<form action='/'>");

        makeNewRow();

        makeTextField("ssid", String(ssid));
        makeTextField("password", "");
        makeButton("SAVE CRED", "/sav_cred");
        makeButton("DELETE CRED", "/del_cred");

        makeNewRow();
        // makeActionForm("/sav_cred");

        makeNewRow();
        makeTextField("DevName", String(deviceName));
        makeTextField("mqttServer", String(mqttServer));
        makeButton("SAVE CONF", "/sav_conf", true);
        makeButton("DELETE CONF", "/del_conf", true);

        makeButton("RESTART", "/restart", true);
        makeButton("UPDATE", "/update");
        // server->sendContent("</form>");

        // makeAlertButton("Test");
        makeReturnButton();
        stopTable();
        stopPage();
    }

    std::function<void()> onSaveCred = [&]() {
        xLogLine("saveCred");
    };

    std::function<void()> onDeleteCred = [&]() {
        xLogLine("deleteCred");
    };

    std::function<void()> onSaveConfig = [&]() {
        xLogLine("saveConfig");
        String content = server->arg("plain");
        Serial.print("received: ");
        Serial.println(content);

        server->sendHeader("Content-Type", "text/plain");
        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->send(200, "text/plain", "responseMessage");
    };

    std::function<void()> onTestConfig = [&]() {
        xLogLine("testConfig");
        String content = server->arg("plain");
        Serial.print("received: ");
        Serial.println(content);
        
        char inputStr[124];
        memcpy(inputStr, content.c_str(), sizeof(inputStr));

        char *ref = strtok(inputStr, " ");
        Serial.print("value1 = "); Serial.println(ref);
        ref = strtok(NULL, " ");
        Serial.print("value2 = "); Serial.println(ref);
        ref = strtok(NULL, " ");
        Serial.print("value3 = "); Serial.println(ref);

        server->send(200, "text/plain", "responseMessage"); 
    };
    
    std::function<void()> handleDevConf = [&]() {
        String content = server->arg("plain");
        Serial.print("received: ");
        Serial.println(content);

        server->sendHeader("Content-Type", "text/plain");
        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->send(200, "text/plain", "responseMessage");
    };

    std::function<void()> handleCORS = [&]() {
        Serial.println("IM HERE OOOOOOOOOOOOOOOOOO");
        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->sendHeader("Access-Control-Max-Age", "10000");
        server->sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        server->sendHeader("Access-Control-Allow-Headers", "Content-Type");
        server->send(204);
    };

    public:
        Web_DevConfig(): Web_Base("Web_Contr") {}

        void begin(Serv_Network *network, WebServer *server) {
            load("/devConf", network, server);

            server->on("/devConf", HTTP_POST, handleDevConf);
            server->on("/devConf", HTTP_OPTIONS, handleCORS);

            server->on("/sav_cred", HTTP_POST, onSaveCred);
            server->on("/saveConf", HTTP_POST, onSaveConfig);
            server->on("/testConf", HTTP_POST, onTestConfig);
        }
};