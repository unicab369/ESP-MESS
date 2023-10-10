// The code below is modification from WebOTA
// link: https://github.com/scottchiefbaker/ESP-WebOTA/tree/master

// MIT License

// Copyright (c) 2019 Scott Baker

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// R Macro string literal https://en.cppreference.com/w/cpp/language/string_literal
const char INDEX_HTML[] PROGMEM = R"!^!(
<!doctype html>
<html lang="en">
<head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<title>WebOTA</title>

	<script src="main.js"></script>
	<link rel="stylesheet" href="main.css">
</head>
<body>
	<h1>OTA Update</h1>

	<form method="POST" action="#" enctype="multipart/form-data" id="upload_form">
		<div><input class="" type="file" name="update" id="file"></div>

		<div><input class="btn" type="submit" value="Upload"></div>
	</form>

	<div>
		<div id="prg" class="prog_bar" style=""></div>
	</div>
</body>
</html>
)!^!";

const char MAIN_CSS[] PROGMEM = R"!^!(
body {
	margin: 2em;
	font-family: sans-serif;
}

input[type=file]::file-selector-button, .btn {
	margin-right: 20px;
	border: 1px solid gray;
	background-image: linear-gradient(to bottom,#59ef59 0, #55982f 100%);
	padding: 10px 20px;
	border-radius: 4px;
	color: #fff;
	cursor: pointer;
	transition: background .2s ease-in-out;
	width: 10em;
}

.btn {
	margin-top: 12px;
	background-image: linear-gradient(to bottom,#78addb 0,#2d6ca2 100%);
}

.prog_bar {
	margin: 12px 0;
	text-shadow: 2px 2px 3px black;
	padding: 5px 0;
	display: none;
	border: 1px solid #7c7c7c;
	background-image: linear-gradient(to right,#d2d2d2 0,#2d2d2d 100%);
	line-height: 1.3em;
	border-radius: 4px;
	text-align: center;
	color: white;
	font-size: 250%;
}
)!^!";

const char MAIN_JS[] PROGMEM = R"!^!(
var domReady = function(callback) {
	document.readyState === "interactive" || document.readyState === "complete" ? callback() : document.addEventListener("DOMContentLoaded", callback);
};

domReady(function() {
	var myform = document.getElementById('upload_form');
	var filez  = document.getElementById('file');

	myform.onsubmit = function(event) {
		event.preventDefault();

		var formData = new FormData();
		var file     = filez.files[0];
		if (!file) { return false; }
		formData.append("files", file, file.name);

		var xhr = new XMLHttpRequest();
		xhr.upload.addEventListener("progress", function(evt) {
			if (evt.lengthComputable) {
				var per = Math.round((evt.loaded / evt.total) * 100);
				var prg = document.getElementById('prg');
				var str = per + "%";
				prg.style.width   = str;
				prg.innerHTML = str;
				prg.style.display = "block";
			}
		}, false);
		xhr.open('POST', location.href, true);

		// Set up a handler for when the request finishes.
		xhr.onload = function () {
			if (xhr.status === 200) {
				//alert('Success');
			} else {
				//alert('An error occurred!');
			}
		};

		xhr.send(formData);
	}
});
)!^!";

class Web_OTA: public Web_Base {
    void makeContent() override {
        char buf[1024];
        snprintf_P(buf, sizeof(buf), INDEX_HTML, MY_ESP.boardType);
        server->send(200, "text/html", buf);
    }

    std::function<void()> onUpdate = [&]() {
        HTTPUpload& upload = server->upload();

		#ifdef ESP32
			//# TODO
			// // Open the firmware file for reading
			// File firmwareFile = SPIFFS.open(firmwarePath, "r");
			// if (!firmwareFile) {
			// 	Serial.println("Failed to open firmware file");
			// 	return;
			// }

			// // Initialize the update process
			// if (Update.begin(UPDATE_SIZE_UNKNOWN)) {
			// 	// Stream the firmware data to the Update library
			// 	size_t written = Update.writeStream(firmwareFile);
			// 	if (written == firmwareFile.size()) {
			// 		Serial.println("Firmware data written successfully");

			// 		// Perform the firmware update
			// 		if (Update.end(true)) {
			// 			Serial.println("OTA update successful");
			// 		} else {
			// 			Serial.println("OTA update failed");
			// 		}
			// 	} else {
			// 		Serial.println("Failed to write firmware data");
			// 	}
			// } else {
			// 	Serial.println("OTA update could not begin");
			// }

			// // Close the firmware file
			// firmwareFile.close();
		#else
			if (upload.status == UPLOAD_FILE_START) {
				Serial.printf("Firmware update initiated: %s\r\n", upload.filename.c_str());

				uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

				if (!Update.begin(maxSketchSpace)) { //start with max available size
					Update.printError(Serial);
				}
			} else if (upload.status == UPLOAD_FILE_WRITE) {
				/* flashing firmware to ESP*/
				if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
					Update.printError(Serial);
				}

				// Store the next milestone to output
				uint16_t chunk_size  = 51200;
				static uint32_t next = 51200;

				// Check if we need to output a milestone (100k 200k 300k)
				if (upload.totalSize >= next) {
					Serial.printf("%dk ", next / 1024);
					next += chunk_size;
				}
			} else if (upload.status == UPLOAD_FILE_END) {
				if (Update.end(true)) { //true to set the size to the current progress
					Serial.printf("\r\nFirmware update successful: %u bytes\r\nRebooting...\r\n", upload.totalSize);
				} else {
					Update.printError(Serial);
				}
			}
		#endif
    };

	std::function<void()> onGetJS = [&]() {
		server->send_P(200, "application/javascript", MAIN_JS);
		xLog("IM HERE 1");
		AppPrintHeap();
	};

	std::function<void()> onGetCSS = [&]() {
		server->send_P(200, "text/css", MAIN_CSS);
		xLog("IM HERE 2");
		AppPrintHeap();
	};

    public:
        Web_OTA(): Web_Base("Web_OTA") {}

        void begin(Serv_Network *network, WebServer *server) {
            load("/update", network, server);

            // Handling uploading firmware file
            server->on("/update", HTTP_POST, [&]() {
                // server->send(200, "text/plain", (Update.hasError()) ? "Update: fail\n" : "Update: OK!\n");
                delay(500);
                ESP.restart();
            }, onUpdate);
            
            server->on("/main.js", HTTP_GET, onGetJS);		//! FILE: main.js
            server->on("/main.css", HTTP_GET, onGetCSS);	//! FILE: main.css
        }
};