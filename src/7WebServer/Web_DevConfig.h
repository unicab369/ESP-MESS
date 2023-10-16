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

    std::function<void()> onSaveCred = []() {
        AppPrint("\n[DevConf] saveCred");
        // strcpy(ssid, (server->arg("ssid")).c_str());
        // strcpy(password, (server->arg("password")).c_str());
        // storage->wifiCred.storeCred(ssid, password);
    };

    std::function<void()> onDeleteCred = []() {
        Serial.println("\n[DevConf] deleteCred");
        // storage->wifiCred.deleteData();
    };

    std::function<void()> onSaveConfig = []() {
        Serial.println("\n[DevConf] saveConfig");
        // strcpy(deviceName, (server->arg("DevName")).c_str());
        // strcpy(mqttServer, (server->arg("mqttServer")).c_str());            
        // storage->devConf.storeConf(deviceName, mqttServer);
    };

    std::function<void()> onDeleteConfig = []() {
        Serial.println("\n[DevConf] deleteConfig");
        // storage->devConf.deleteData();
    };
    
    std::function<void()> handleDevConf = [&]() {
        String responseMessage = "Received a POST request to /devConf";
        server->sendHeader("Content-Type", "text/plain");

        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->send(200, "text/plain", responseMessage);

        // Print the response message to the Serial Monitor
        Serial.println(responseMessage);
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
            server->on("/del_cred", HTTP_POST, onDeleteCred);
            server->on("/sav_conf", HTTP_POST, onSaveConfig);
            server->on("/del_conf", HTTP_POST, onDeleteConfig);
        }
};