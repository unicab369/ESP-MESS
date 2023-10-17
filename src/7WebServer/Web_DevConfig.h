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
        String content = server->arg("plain");
        char* chars = (char*)content.c_str();
        xLogLinef("onSaveCred Received = %s", chars);

        server->send(200, "application/json", "{\"ok\": true}"); 
    };

    std::function<void()> onDeleteCred = [&]() {
        xLogLine("deleteCred");

        server->send(200, "application/json", "{\"ok\": true}"); 
    };

    //! saveConf
    std::function<void()> onSaveConfig = [&]() {
        String content = server->arg("plain");
        char* chars = (char*)content.c_str();
        xLogLinef("onSaveConfig Received = %s", chars);

        server->sendHeader("Content-Type", "text/plain");
        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->send(200, "application/json", "{\"ok\": true}"); 
    };

    //! testConf
    std::function<void()> onTestConfig = [&]() {
        String content = server->arg("plain");
        char* chars = (char*)content.c_str();
        xLogLinef("onTestConfig Received = %s", chars);

        char *ref = strtok(chars, " ");
        Serial.print("value1 = "); Serial.println(ref);
        ref = strtok(NULL, " ");
        Serial.print("value2 = "); Serial.println(ref);
        ref = strtok(NULL, " ");
        Serial.print("value3 = "); Serial.println(ref);

        server->send(200, "application/json", "{\"ok\": true}"); 
    };
    
    //! devConf
    std::function<void()> handleDevConf = [&]() {
        String content = server->arg("plain");
        char* chars = (char*)content.c_str();
        xLogLinef("handleDevConf Received = %s", chars);

        server->sendHeader("Content-Type", "text/plain");
        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->send(200, "application/json", "{\"ok\": true}"); 
    };

    //! CORS
    std::function<void()> handleCORS = [&]() {
        Serial.println("IM HERE OOOOOOOOOOOOOOOOOO");
        server->sendHeader("Access-Control-Allow-Origin", "*");
        server->sendHeader("Access-Control-Max-Age", "10000");
        server->sendHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        server->sendHeader("Access-Control-Allow-Headers", "Content-Type");
        server->send(204);
    };

    //! saveMac
    std::function<void()> onSaveMac = [&]() {
        String content = server->arg("plain");
        char* chars = (char*)content.c_str();
        xLogLinef("onSaveMac Received = %s", chars);

        uint8_t myChar[6];
        int i = 0;
        char* token = strtok(chars, ":");
        while (token != NULL && i < 6) {
            myChar[i++] = (uint8_t)strtol(token, NULL, 16);
            token = strtok(NULL, ":");      // Split the string into tokens based on ":"
        }

        xLogStatus("ValidFormat", i==6);
        server->send(200, "application/json", "{\"ok\": true}"); 
    };

    //! saveGroup
    std::function<void()> onSaveGroup = [&]() {
        String content = server->arg("plain");
        char* chars = (char*)content.c_str();
        xLogLinef("onSaveGroup Received = %s", chars);
        
        server->send(200, "application/json", "{\"ok\": true}"); 
    };

    public:
        Web_DevConfig(): Web_Base("Web_Contr") {}

        void begin(Serv_Network *network, WebServer *server) {
            load("/devConf", network, server);

            server->on("/sav_cred", HTTP_POST, onSaveCred);

            //! devConf
            server->on("/devConf", HTTP_POST, handleDevConf);
            server->on("/devConf", HTTP_OPTIONS, handleCORS);
            server->on("/saveConf", HTTP_POST, onSaveConfig);
            server->on("/testConf", HTTP_POST, onTestConfig);
            server->on("/saveMac", HTTP_POST, onSaveMac);
            server->on("/saveGroup", HTTP_POST, onSaveGroup);
        }
};