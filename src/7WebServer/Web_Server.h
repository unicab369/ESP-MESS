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

class Web_Server: public Loggable {
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
    HTTPClient http;
    const char* serverUrl = "http://iotplotter.com/api/v2/feed/";

    public:
        Web_Server(): Loggable("Web_Server") {}

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

        // iotPlotter <apiKey> <feedId>
        // selfPlot <enable>
        void makePostRequest(const char* apiKey, const char* feed, float temp, float hum, float lux, float volt, float mA) {
            if (strlen(apiKey) < 32 || strlen(feed) < 10) return;
            
            http.begin(String(serverUrl) + String(feed));
            http.addHeader("api-key", apiKey);
            http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            
            String postData = "{\"data\":{\"Temp\": [{\"value\":" + String(temp) + 
                                "}], \"Hum\":[{\"value\":" + String(hum) + 
                                "}], \"Lux\":[{\"value\":" + String(lux) + 
                                "}], \"Volt\":[{\"value\":" + String(volt) +
                                "}], \"mA\":[{\"value\":" + String(mA) + "}] }}";
            // String postData = "{\"data\":{\"Temperature\":[{\"value\":25.05},{\"value\":25.99,\"epoch\":1516195980},{\"value\":24.99,\"epoch\":1516195280}],\"ANY_GRAPH_NAME_HERE\":[{\"value\":123}]}}";
            // postData += String(temp) + "}], \"ANY_GRAPH_NAME_HERE\":[{\"value\":123}]}}"
            int responseCode = http.POST(postData);
            xLogf("responseCode: %d", responseCode);
        }
        
        void refreshReadings(int temp, int hum, int lux) {
            // WebSerial.println(WiFi.localIP().toString());
        }

        void run() {
            webServer.handleClient();
        }
};

