char HTTP_WEB_START[] = R"!^!(<!DOCTYPE html>
<html><head>
<script src='root.js'></script>
<link rel='stylesheet' href='button.css'>
<link rel='stylesheet' href='forms.css'>
</head><body>
<div style='margin: auto; width: 640px; background-color: MediumSeaGreen;'>
)!^!";

char HTTP_WEB_END[] = "</div></body></html>";

char MAIN_JS2[] = R"!^!(
function goBack() {
   console.log('Button clicked');
   window.history.back();
}
function showAlert() {
   var xhr = new XMLHttpRequest();
   xhr.onreadystatechange = function() {
      if (xhr.readyState === 4 && xhr.status === 200) {
         alert('Read Value: ' + xhr.responseText);
      }
   };
   xhr.open('GET', '/analogRead', true);
   xhr.send();
}
function sendPostRequest(path, isPost) {
   console.log('IM HERE sendPostRequest');
   var xhr = new XMLHttpRequest();
   xhr.onreadystatechange = function() {
      if (xhr.readyState === 4 && xhr.status === 200) {
      }
   };
   xhr.open(isPost ? 'POST' : 'GET', path, true);
   xhr.send();
}
)!^!";



#define HTML_BUFFER_SIZE 1024

class Web_Base: public Loggable {
   private:
      char* buffer = new char[HTML_BUFFER_SIZE];
      int bufferSize = HTML_BUFFER_SIZE;

   void appendStr(const char* str) {
      int currentLen = strlen(buffer);
      int appendLen = strlen(str);

      if (currentLen + appendLen >= bufferSize) {
         // Calculate the new buffer size (e.g., double the size)
         bufferSize = (currentLen + appendLen + 1) * 2; // +1 for the null terminator

         // Allocate a new buffer and copy the existing content
         char* newBuffer = new char[bufferSize];
         strcpy(newBuffer, buffer);    

         delete[] buffer;        // Clean up the old buffer
         buffer = newBuffer;     // Update the buffer pointer          
      }
      
      strcat(buffer, str);   // Append the additional content
   }

   protected:
      virtual void makeContent() {}
      char dirPath[20];
      Serv_Network* network;
      WebServer* server;

      std::function<void(void)> handleBaseContent = [&]() {
         makeContent();
      };

   public:
      Web_Base(const char* name): Loggable(name) {}

      const char* getPath() { return (const char*)dirPath; }

      void load(const char* path, Serv_Network *_network, WebServer *_server) {
         memcpy(dirPath, path, sizeof(dirPath));
         server = _server;
         network = _network;
         server->on(dirPath, HTTP_GET, handleBaseContent);
      }

      void startPage() {
         server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
         server->sendHeader("Pragma", "no-cache");
         server->sendHeader("Expires", "-1");
         
         //! STRICTLY REQUIRED for correct initialization of string
         buffer[0] = '\0';
         appendStr(HTTP_WEB_START);
      }

      void stopPage() {
         appendStr(HTTP_WEB_END);
         webServer.send(200, "text/html", buffer);
      }

      void startTable(String title) {
         // response->printf("<h1>%s</h1>", title);   
         appendStr("<table style='width:100%'; font-size:60px>");
         String str = "<tr><th colspan='2'>" + title + "</th></tr>";
         appendStr(str.c_str());
      }

      void stopTable() {
         appendStr("</table>");
      }

      void makeNewRow() {
         appendStr("<tr><td>&nbsp;</td></tr>");
      }

      void makeActionForm(const char* path) {
         String str = "<form action='" + String(path) + "' method='post>";
         appendStr(str.c_str());
         makeTextArea("name", "TestName");
         appendStr("<td colspan='2'><input type='submit' value='Submit1'></td>");
         appendStr("</form>");
      }

      void makeTableRow(const char* key, const char* value) {
         appendStr("<tr>");
         String str = "<td>" + String(key) + ":</td>" + 
                           "<td>" + String(value) + "</td>";
         appendStr(str.c_str());
         appendStr("<t/r>");
      }

      void makeTableRow(const char* key, String value) {
         makeTableRow(key, value.c_str());
      }

      void makeTableRow(const char* key, uint64_t value) {
         char val[22];
         sprintf(val, "%lu", value);
         makeTableRow(key, val);
      }
      
      void makeFormButton(String title, String action) {
         makeNewRow();
         String output = "<tr><td colspan='2'><input type='submit' value='" + title +  
               "' formaction='" + action + "' style='display:inline-block; width:100%; height:100px; font-size:40px;'></td></tr>";
         appendStr(output.c_str());
      }

      void makeRow(const char* content) {
         appendStr("<tr>");
         appendStr(content);
         appendStr("</tr>");
      }

      void makeAlertButton(String title) {
         String btnStr = "<td colspan='2'><button onclick='showAlert()' style='display:inline-block; width:100%; height:100px; font-size:40px;'>" 
                              + title + "</button></td>";
         appendStr("<tr>");
         appendStr(btnStr.c_str());
         appendStr("</tr>");
      }

      String buttonStyle = "class='pure-button FontLarge WidthFull Height100px'";

      //! NOTE: button title and href string length is limited
      void makeButton(String title, const char* href, bool isPost = false) {
         makeNewRow();
         String btnStr;

         if (isPost) {
               String isPostString = isPost ? "true" : "false";
               btnStr = "<td colspan='2'><button type='button' onclick=\"sendPostRequest('" + String(href) + 
                     "'," + isPostString + ")\" " + buttonStyle + ">" + title + "</button></td>";
         } else {
               btnStr = "<td colspan='2'><form action='" + String(href) + "'>" + 
                  "<button " + buttonStyle + ">" + title + "</button></form></td>";
         }

         makeRow(btnStr.c_str());
      }

      void makeReturnButton(const char* title = "BACK") {
         makeNewRow();
         String btnStr = "<td colspan='2'><button onclick='goBack()'" + buttonStyle + ">" + String(title) + "</button></td>";
         makeRow(btnStr.c_str());         
      } 

      void makeTextArea(String key, String value) {
         String str = "<tr><td colspan='2'><textarea name='" + key + 
                     "' cols='40' rows='1' maxlength='64' style='width:98%; height:50px; resize:none; font-size:30px;'>" + value + "</textarea></td></tr>";
         appendStr(str.c_str());
      }

      void makeTextField(String key, String value) {
         String str = "<div class='pure-control-group'><td width='30%'>\
         <label for='tf-name' class='Width200px FontLarge'>" + key + "</label></td>\
         <td width='100%'><input type='text' id='tf-name' class='Width95 FontLarge' placeholder='" + key + "' /></td></div>";
         appendStr("<tr>");
         appendStr(str.c_str());
         appendStr("</tr>");
      }
};