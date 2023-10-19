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

        uint8_t intValue;
        uint8_t mac[6];
        int result = sscanf(chars, "%hhu %2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", 
                    &intValue, &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
        xLogf("IntValue = %hhu, result = %u", intValue, result);

        if (result==7 && AppCheckMac(mac)) {
            network->device->storage.stoPeer.insertPeer(mac, intValue);
        }

        server->send(200, "application/json", "{\"ok\": true}"); 
    };

    //! saveGroup
    std::function<void()> onSaveGroup = [&]() {
        String content = server->arg("plain");
        char* chars = (char*)content.c_str();
        xLogLinef("onSaveGroup Received = %s", chars);
        
        server->send(200, "application/json", "{\"ok\": true}"); 
    };

    //! saveConf
    std::function<void()> onSaveConfig = [&]() {
        String content = server->arg("plain");
        char* chars = (char*)content.c_str();
        xLogLinef("onSaveConfig Received = %s", chars);


        uint8_t targetId;
        char triggerChar[20], actionChar[20];
        uint8_t pinCode;
        uint8_t pinValue;
        
        int result = sscanf(chars, "%hhu %19s %19s %hhu %hhu", 
                        &targetId, &triggerChar, &actionChar, &pinCode, &pinValue);

        if (result == 5) {
            xLogf("validated: %s %s", triggerChar, actionChar);
            BehaviorItem behav_In;

            if (strcmp(actionChar, "OUTPUT") == 0) {
                ControlOutput output(pinCode, pinValue);
                behav_In.load(targetId, (const char*)triggerChar, &output);
                // stoBehav.updateData(behavIndex, &behav_In);     //! store behavior
            } 
            else if (strcmp(actionChar, "WS2812") == 0) {
                ControlWS2812 output(pinCode, pinValue);

            }
            else if (strcmp(actionChar, "PUBLISH") == 0) {

            }
            else {
                //! delete
            }
        }


        server->send(200, "application/json", "{\"ok\": true}"); 
    };

    //! testConf
    std::function<void()> onTestConfig = [&]() {
        String content = server->arg("plain");
        char* chars = (char*)content.c_str();
        xLogLinef("onTestConfig Received = %s", chars);

        uint8_t targetId;
        char action[20];
        uint8_t pinCode;
        uint8_t pinValue;
        
        int result = sscanf(chars, "%hhu %19s %hhu %hhu", &targetId, &action, &pinCode, &pinValue);

        if (result == 4) {
            xLog("validated: %s", action);

            if (strcmp(action, "OUTPUT") == 0) {
                ControlOutput output(pinCode, pinValue);
                network->device->handleAction(output);
            } 
            else if (strcmp(action, "WS2812") == 0) {
                ControlWS2812 output(pinCode, pinValue);
            }
            else if (strcmp(action, "PUBLISH") == 0) {

            }
            else {
                //! delete
            }
        }

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