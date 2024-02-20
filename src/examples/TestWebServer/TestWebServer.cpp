/* NOTE: Create a TestConfig.h file under the /examples
   copy the following lines to the file, update the variables with your wifi credential

   #define WIFI_SSID "MY_SSID"
   #define WIFI_PASS "MY_PASSWORD"

   commands:
   pio run --target buildfs --environment ESP8266-TestWebServer
   pio run --target uploadfs --environment ESP8266-TestWebServer

   http://xx.xx.xx.xx/pages/page1.html
   http://xx.xx.xx.xx/style/style1.html
*/

#include "examples/TestConfig.h"

// https://shepherdingelectrons.blogspot.com/2019/04/esp8266-as-spiffs-http-server.html

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
// #include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>   // Include the SPIFFS library

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

String getContentType(String filename); // convert the file extension to the MIME type
bool handleFileRead(String path);       // send the right file to the client (if it exists)

void setup() {
   Serial.begin(115200);         // Start the Serial communication to send messages to the computer
   delay(10);
   Serial.println('\n');

   wifiMulti.addAP(WIFI_SSID, WIFI_PASS);   // add Wi-Fi networks you want to connect to

   Serial.println("Connecting ...");
   int i = 0;
   while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
      Serial.print('.');
      delay(250);
   }

   Serial.println('\n');
   Serial.print("Connected to "); Serial.println(WiFi.SSID()); 
   Serial.print("IP address:\t"); Serial.println(WiFi.localIP());

   // if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
   //    Serial.println("mDNS responder started");
   // } else {
   //    Serial.println("Error setting up MDNS responder!");
   // }

   SPIFFS.begin();                           // Start the SPI Flash Files System
   
   server.onNotFound([]() {                              // If the client requests any URI
      if (!handleFileRead(server.uri()))                  // send it if it exists
         server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
   });

   server.begin();                           // Actually start the server
   Serial.println("HTTP server started");
}

void loop(void) {
   server.handleClient();
}

String getContentType(String filename) { // convert the file extension to the MIME type
   if (filename.endsWith(".html")) return "text/html";
   else if (filename.endsWith(".css")) return "text/css";
   else if (filename.endsWith(".js")) return "application/javascript";
   else if (filename.endsWith(".ico")) return "image/x-icon";
   return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
   Serial.println("handleFileRead: " + path);
   if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
   String contentType = getContentType(path);            // Get the MIME type
   if (SPIFFS.exists(path)) {                            // If the file exists
      File file = SPIFFS.open(path, "r");                 // Open it
      size_t sent = server.streamFile(file, contentType); // And send it to the client
      file.close();                                       // Then close the file again
      return true;
   }
   Serial.println("\tFile Not Found");
   return false;                                         // If the file doesn't exist, return false
}