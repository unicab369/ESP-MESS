# esp_mess
esp_mess is an open-source firmware for ESP8266 and ESP32 microcontrollers, enabling rapid IoT prototyping for home automation, smart farming, smart business, and more. 

checkout our discord channel: 
https://discord.com/channels/793348436475904022/858758498875342878

need to create

src/1Configs/Conf_User.h:
#define MY_SSID "YOUR_WIFI_SSID"
#define MY_PASS "YOUR_WIFI_PASSWORD"

# npm install
https://nodejs.org/en
cmd: npm install -g live-server
cmd: live-server
Run PowerShell as Administrator
cmd: Set-ExecutionPolicy Unrestricted
answer: Y

# plugins

https://serial.huhn.me/

PlatformIO IDE
Better Comments
REST Client
Serial Monitor
Live Preview
Live Server

# General Shortcuts
Ctrl+C/V: Copy/Paste
Ctrl+S/O: Save/Open
Ctrl+W: Close Tab or Window
Ctrl+Z: Undo
Ctrl+F: Find
Ctrl+A: Select All
Ctrl+N: New Window

Alt+Tab: Switch between windows
Win+Left/Right: Snap-in window (in Multitasking turn off checkboxes)
Win+Number: Switch between apps in order
Win+S: Search
Win+X: Secret Start Menu
Win+E: File Explorer
Win+D: Minimize All

# Browser Shortcuts
Ctrl+T: New Tab 
Ctrl+Number: Switch tab Number

# YouTube Shortcuts
up/down: increase/decrease volume
left/right: Seek back and forth
Shift+<: reduce playback rate
Shift+>: increase playback rate
Spacebar: Play/Pause
c: caption on or off
i: open mini player
Shift+N: next video
Shift+P: previous video

# VsCode Shortcuts
Ctrl+/: Comment out Selected
Ctrl+Click: click on variable or method to see definitions
Ctrl+G: Goto line
PlatformIO: Ctrl+Alt+U: Upload code

# Window Screenshots
Snipping Tools: Search for Snipping Tools app
Snipping & Sketch: Win+Shift+S

# PiHole Blocklists
https://firebog.net/


# App Flow
```mermaid
  graph TD;
      Serv_Device-->Serv_Network; Serv_Network-->Net_Wifi;
      Serv_Network-->Web_Server;
      Web_Server-->Web_Root;
```

https://github.com/SensorsIot/IOTstack/blob/master/docs/Containers/InfluxDB.md#authentication