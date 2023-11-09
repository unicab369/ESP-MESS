# E104-BT5011A

### Commands
AT<br>
AT+RESET<br>
AT+RESTORE<br>
AT+ROLE?= 0 for Slave, 1 for Master, 2 for Observer<br>
AT+ADV? Advertise: = 0 for Disable, 1 for Enable<br>
AT+ADVINTV? Advertise: = 1600 for 1 second (range from 32 to 16384)<br>
AT+NAME?=[deviceName] max 22 bytes<br>
AT+CONPARAMS? Connection Gap: = [intv], [latency], [timeout]<br>
AT+DISCON<br>
AT+PEERMAC?<br>
AT+BOND?= 0 for Disable, 1 for Enable<br>
AT+BONDMAC?<br>
AT+BONDDEL= 6bytes mac address to be removed. delete all 0xff, 0xff, 0xff, 0xff, 0xff, 0xff<br>
AT+AUTH=[password] 6 bytes max<br>
AT+UAUTH?=[password] 6 bytes max<br>
AT+LOGMSG?= 0 for Disable, 1 for Enable<br>
AT+SLEEP<br>
AT+PWR?=[power] value 0 (4dBm) to 8 (-40dBm)<br>

AT+MAC?<br>
AT+SCANINTV?=[intv], [wnd]<br>
AT+FNAME?=[enable], [name]<br>
AT+FRSSI?=[enable], [pwr] pwr value from -128 to 127<br>
AT+PHY?= 0 for 1M PHY, 1 for 2M PHY, 2 for CODE PHY<br>

### Power Consumption
- Active current: average 475uA, max 930uA<br>
- Sleeping current: average 1.77uA, max 8.78uA<br>
- Transmitting current 4dBm: average 2.22mA, max 6.7mA<br>
- Transmission window 4dBm: 6.5ms<br>
- Transmitting current -40dBm: average 2.10mA, max 5.15mA<br>
- Transmission window -40dBm: 5.4ms<br>

# E104-BT5010A

### Commands
AT+IPWR?= -128~127<br>
AT+ADV? Advertise: = 0 for Disable, 1 for Enable, 2 for iBeacon<br>
AT+DEVMANUF?= max 32 bytes<br>
AT+DEVMODEL?= max 32 bytes<br>
AT+DEVHWVER?= max 32 bytes<br>
AT+DEVSWVER?= max 32 bytes<br>
AT+DEVID?= max 8bytes<br>
AT+ADVDAT?= max 26 bytes<br>
AT+IBCNUUID?= max 16 bytes<br>
AT+MAJOR?= 1-65535<br>
AT+MINOR?= 1-65535<br>
AT+SCAN?= 0 for Disable, 1 for Enable<br>
AT+SCANINTV?= 4-65535 (x*0.625=ms)<br>
AT+SCANWND?= 4-65536 (x*0.625=ms)<br>
AT+AUTOCONN?= 0 for Disable, 1 for Enable<br>
AT+CONN?=[mac]<br>
AT+UUIDSVR?=1-65535<br>
AT+UUIDCHARA1?=1-65535<br>
AT+UUIDCHARA2?=1-65535<br>
AT+AUTH= 6 bytes max

### Power Consumption
- Active current: average 548uA, max 870uA<br>
- Sleeping current: average 1.77uA, max 8.78uA<br>
- Transmitting current 4dBm: average 2.52mA, max 6.4mA<br>
- Transmission window 4dBm: 4.5ms<br>
- Transmitting current -40dBm: average 2.0mA, max 4.56mA<br>
- Transmission window -40dBm: 4.2ms<br>

