#ifdef ESP32
    #include <WebServer.h>
    #include <HTTPClient.h>

#else
    #include <ESP8266WebServer.h>
    #include <ESP8266HTTPClient.h>

    class WebServer: public ESP8266WebServer {
        public:
            WebServer(int port) : ESP8266WebServer(port) {}
    };
#endif

WebServer webServer(80);

#include "Web_Root.h"

char WebServer_timeString[50];
long WebServer_resetCnt = 0;
char WebServer_devName[32] = "NULL";

class Web_Server: public BaseComponent {
    void WebServer_Response() {
        Serial.print("\n[WebServer] ");
        webServer.sendHeader("Access-Control-Allow-Origin","*");
        webServer.send(200, "text/plain", "{\"result\": \"OK\"}" );
    }

    void WebServer_Response(char *data, int len) {
        WebServer_Response();
        String argData = webServer.arg("plain");
        argData.toCharArray(data, len);
    }

    int value = 0;

    public:
        Web_Server(): BaseComponent("Base") {}

        Web_Root webRoot;
        Serv_Network *network;

        void setup(Serv_Network *_network) {
            network = _network;

            //! Setup webRoot: path /mess
            webRoot.begin(network, &webServer);

            //! REQUIRED: otherwise crash if Wifi hasn't begin
            if (WiFi.status() != WL_NO_SHIELD) {
                webServer.begin();
            }
        }

        void refreshReadings(int temp, int hum, int lux) {
            // WebSerial.println(WiFi.localIP().toString());
        }

        void run() {
            webServer.handleClient();
        }
};

