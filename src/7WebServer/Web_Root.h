
#include "Web_Base.h"
#include "Web_DevConfig.h"
#include "Web_ControlConfig.h"
#include "Web_DevFile.h"
#include "Web_OTA.h"

class Web_Root: public Web_Base {
    void makeContent() override {
        char deviceName[21] = "", mqttServer[21] = "";
        // device->storage.devConf.loadConf(deviceName, mqttServer);
        startPage();        
        startTable("ESP_Mess V3");

        makeNewRow();
        makeTableRow("Built Date", String(__DATE__) + " @ " + String(__TIME__));
        // makeTableRow("Run Time", network->device->asyncTimer1.record());
        // makeTableRow("Time Keeping", network->device->appClock.getDateTimeStr());
        // makeTableRow("Reset Count", network->device->storage.resetCount.value);
        makeTableRow("Reset Reason", String(MY_ESP.getResetReason()));

        makeNewRow();
        makeTableRow("DevName", deviceName);
        makeTableRow("Hostname", network->wifi.hostName);
        makeTableRow("Network", WiFi.SSID() + " (" + String(WiFi.RSSI()) + " dBm)");
        makeTableRow("MAC Addr", WiFi.macAddress());

        makeNewRow();
        makeTableRow("MCU ChipID", MY_ESP.chipIdHex());
        makeTableRow("CPU Freq", String(MY_ESP.cpuFreq()) + " MHz");
        makeTableRow("Flash Size", String(MY_ESP.flashSize()) + " kB");
        
        //! sketchUsed takes 1114ms, sketchTotal takes 1059ms
        // makeTableRow("Sketch Size", String(MY_ESP.sketchUsed()) +   
        //                 "/" + String(MY_ESP.sketchTotal())  + " kB");
        // // makeTableRow("Heap Size", String(params.heapUsed) + 
        // //                 "/" + String(params.heapTotal) + " kB");

        makeNewRow();
        makeButton("TOGGLE IO", "/toggleIO", true);
        makeButton("DEVICE CONFIG", devConfig.getPath());
        makeButton("CONTROL CONFIG", ctrlConfig.getPath());
        makeButton("FILES", devFile.getPath());

        stopTable();
        stopPage();
        AppPrintHeap();
    }

    Web_DevConfig devConfig;
    Web_ControlConfig ctrlConfig;
    Web_DevFile devFile;
    Web_OTA webOTA;
    Serv_Network *network;

    std::function<void()> handleRestart = []() {
        MY_ESP.restart(); 
    };

    std::function<void()> handleToggle = [&]() {
        xLog("IM HERE 111");
        network->interface->toggleRelay();
        server->send(200, "text/plain", "Hello!"); 
    };

    std::function<void()> handleSetTime = [&]() {
        // int hr; int min; int sec; int day; int month; int year;
        // sscanf(argData.c_str(), "%d, %d, %d, %d/%d/%d", &hr, &min, &sec, &day, &month, &year);
        // Serial.printf("%d:%d:%d %d-%d-%d", hr, min, sec, day, month, year);
    };

    std::function<void()> handleSendRF = [&]() {
        // sscanf(argData.c_str(), "%2hhx, %2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx, %2hhx, %d, %d", 
        //         &payload[0], &dest[0], &dest[1], &dest[2], &dest[3], &dest[4], &dest[5], 
        //             &payload[1], &target, &value);
        // payload[2] = lowByte(target);
        // payload[3] = lowByte(value);
    };
    
    std::function<void()> handleJs = [&]() {
        xLog("IM HERE yyyyyy");
        server->send(200, "application/javascript", MAIN_JS2);
    };

    std::function<void()> pureCssButton = [&]() {
        server->send(200, "text/css", HTTP_BUTTONS_CORE);
    };

    std::function<void()> pureCssForm = [&]() {
        server->send(200, "text/css", HTTP_FORMS);
    };

    void sendFileResponse(const char* filePath, const char* contentType) {
        WiFiClient client = server->client();
        File file = LittleFS.open(filePath, "r");
        if (file) {
            client.println("HTTP/1.1 200 OK");
            client.printf("Content-Type: %s\r\n", contentType);
            client.println("Connection: close");
            client.println();
            
            while (file.available()) {
                client.write(file.read());
            }
            file.close();
        } else {
            client.println("HTTP/1.1 404 Not Found");
            client.println("Connection: close");
            client.println();
        }
    }

    std::function<void()> uPlotJs = [&]() {
        // String content = readFileIntoString("/uPlot.iife.min.js");
        // server->send(200, "application/javascript", UPLOT_JS);
        WiFiClient client = server->client();
        client.println("HTTP/1.1 200 OK");
        client.printf("Content-Type: text/javascript\r\n");
        client.println("Connection: close");
        client.println(UPLOT_JS);
    };

    std::function<void()> uPlotCss = [&]() {
        server->send(200, "text/css", UPLOT_CSS);
        // sendFileResponse("/uPlot.min.css", "text/css");
    };

    std::function<void()> uPlotHtml = [&]() {
        server->send(200, "text/html", UPLOT_HTML);
        // sendFileResponse("/align-data.html", "text/html");
    };

    String readFileIntoString(const char* filePath) {
        String content = "";

        File file = LittleFS.open(filePath, "r");
        if (file) {
            while (file.available()) {
                Serial.println("reading...");
                content += (char)file.read();
            }
            file.close();
        }

        return content;
    }

    public:
        Web_Root(): Web_Base("Web_Root") {}

        void begin(Serv_Network *_network, WebServer *_server) {
            xLog(__func__);

            network = _network;
            server = _server;
            load("/", network, server);
            server->onNotFound(handleBaseContent);

            // devConfig.begin(network, server);
            // ctrlConfig.begin(network, server);
            // devFile.begin(network, server);
            webOTA.begin(network, server);
            
            // server->on("/uPlot.iife.min.js", HTTP_GET, uPlotJs);
            // server->on("/uPlot.min.css", HTTP_GET, uPlotCss);
            // server->on("/testPlot", HTTP_GET, uPlotHtml);

            // server->on("/root.js", HTTP_GET, handleJs);
            // server->on("/button.css", HTTP_GET, pureCssButton);
            // server->on("/forms.css", HTTP_GET, pureCssForm);

            // server->on("/restart", HTTP_POST, handleRestart);
            // server->on("/toggleIO", HTTP_POST, handleToggle);
            // server->on("/setTime", handleSetTime);
            // server->on("/sendRF", handleSendRF);
        }
};