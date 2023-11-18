| Left |  Center  | Right |
|:-----|:--------:|------:|
| L0   | **bold** | $1600 |
| L1   |  `code`  |   $12 |
| L2   | _italic_ |    $1 |


# E104-BT5011A

### Commands
AT+SLEEP<br>
AT+PWR?=[power] value 0 (4dBm) to 8 (-40dBm)<br>
AT+LOGMSG?= 0 for Disable, 1 for Enable<br>

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

AT+MAC?<br>
AT+SCANINTV?=[intv], [wnd]<br>
AT+FNAME?=[enable], [name]<br>
AT+FRSSI?=[enable], [pwr] pwr value from -128 to 127<br>
AT+PHY?= 0 for 1M PHY, 1 for 2M PHY, 2 for CODE PHY<br>

### Power Consumption
- Active current: average 475uA, max 930uA<br>
- Sleeping current: average 1.77uA, max 8.78uA<br>
- Transmitting current 4dBm: average 2.22mA, max 6.7mA, dur 6.5ms<br>
- Transmitting current -40dBm: average 2.10mA, max 5.15mA, dur 5.4ms<br>

# E104-BT5010A

### Commands
AT+IPWR?= -128~127<br>
AT+ADV? Advertise: = 0 for Disable, 1 for Enable, 2 for iBeacon<br>
AT+ADVDAT?= max 26 bytes<br>
AT+ADVDAT1?= max 26 bytes<br> set but do not save
AT+ADVINTV?= 1600 (1second) // value*0.625=second // 32-16384
AT+SCAN?= 0 for Disable, 1 for Enable<br>
AT+SCANINTV?= 4-65535 (x*0.625=ms)<br>
AT+SCANWND?= 4-65536 (x*0.625=ms)<br>
AT+DEVID?= max 8bytes<br>

AT+DEVMANUF?= max 32 bytes<br>
AT+DEVMODEL?= max 32 bytes<br>
AT+DEVHWVER?= max 32 bytes<br>
AT+DEVSWVER?= max 32 bytes<br>
AT+IBCNUUID?= max 16 bytes<br>
AT+MAJOR?= 1-65535<br>
AT+MINOR?= 1-65535<br>
AT+AUTOCONN?= 0 for Disable, 1 for Enable<br>
AT+CONN?=[mac]<br>
AT+UUIDSVR?=1-65535<br>
AT+UUIDCHARA1?=1-65535<br>
AT+UUIDCHARA2?=1-65535<br>
AT+AUTH= 6 bytes max

### Power Consumption
- Active current: average 548uA, max 870uA<br>
- Sleeping current: average 1.77uA, max 8.78uA<br>
- Trans. current BLE 4dBm: average 2.52mA, max 6.4mA, dur 4.5ms<br>
- Trans. current BLE -40dBm: average 2.0mA, max 4.56mA, dur 4.2ms<br>

- Trans. current IBEACON 4dBM -128: average 3.13mA, max 8.03mA, dur 4.3ms<br>
- Trans. current IBEACON 4dBm 127: average 2.85mA, max 7.55mA, dur 4.3ms<br>
- Trans. current IBEACON -40dBM -128: average 1.60mA, max 3.10mA, dur 4.0ms<br>
- Trans. current IBEACON -40dBM 127: average 1.90mA, max 4.50mA, dur 4.0ms<br>


# JDY-23
### Commands
AT+VER<br>
AT+RST<br>
AT+BAUD<br>
AT+NAME<br>
AT+STAT<br>
AT+DISC<br>
AT+MAC<br>
AT+DEFAULT<br>

AT+HOSTEN(mode: 0 transparent, 3 Slave iBeacon)<br>
AT+SLEEP(sleepMode: 1 light sleep, 2 deep sleep no broadcast)<br>
AT+STARTEN(mode: 1 power on to wake up, 0 power on sleep/connect wakeup/disconnect sleep)<br>
AT+ADVIN(delay: x[0-9]*100+100ms)<br>
AT+POWR(NOT WORKING?)<br>
AT+ENLOG(enable: 1 for enable, 0 for disable)<br>
AT+ALED(enabled: 0 broadcast LED off, 1 broadcast LED indicator on)<br>
AT+MTU(mode: 1 for 20 bytes, 2 for 128 bytes)<br>
AT+BATT(0-99)<br>

AT+IBUUID(32 bytes max?)<br>
AT+MAJOR(0000-FFFF)<br>
AT+MINOR(0000-FFFF)<br>
AT+IBSING(00-FF)<br>

### Observed Behaviors
1. Deep Sleep no broadcast (mode 2): 
   - many short spikes, max spikes use 10.65mA and last ~700uSec long.
   - average current draw 9.10uA (advertise interval 100ms);
   - average current draw 8.95uA (advertise interval 1000ms).

2. Light Sleep (mode 1):
   - many short spikes, broadcast spike average 2.95mA, max 10mA, duration ~9.5ms.
   - average current draw 135uA (advertise interval 200ms).
   - average current draw 60uA (advertise interval 500ms).
   - average current draw 35uA (advertise interval 1000ms).

3. Active (No Connection):
   - broadcast spike average 3.8mA, max 10mA, duration ~6.4ms.
   - average current draw 813uA (advertise interval 200ms).
   - average current draw 753uA (advertise interval 500ms).
   - average current draw 732uA (advertise interval 1000ms);

4. Active (with Connection):
   - broadcast spike average 9.40mA, max 15mA, duration ~4.3ms (adv interval 100ms).

3. Others:
   - required reset for changes to take place
   - only 2 features UUID usable, one fir transparent transmission and the battery level
   - the chip I have the firmware doesn't allow read and write on the IOs


# JDY-25
### Commands
AT<br>
AT+RESET<br>
AT+VERSION<br>
AT+LADDR<br>
AT+NAME (18 bytes max)<br>
AT+PIN (4 bit password)<br>
AT+TYPE (0 no password, 1 password no binding, 2 password with binding)<br>
AT+BAUD (4 9600, 8 115200)<br>
AT+DISC<br>
AT+STAT (0 unconnected, 1 connected, 2 networked, 3 connected and networked)<br>
AT+STARTEN (1 power on wakeup, 0 power on sleep - connect wakeup - disconnect sleep)<br>
AT+ADVIN (# * 100ms)<br>
AT+POWR (0 -15dBm, 1 -0dBm, 2 2dBm, 3 3dBm, 4 4dBm)<br>
AT+ROLE (0 transparent, 1 master, 2 BLE broadcast, 3 iBeacon mode, 4 iBeacon probe, 5 MESH,
6 Multi-connected slave supports 4 masters connections, 7 Multi-connected master-slave master supports to connect 4 slateves at the same time, and slave supports to connect 4 masters, 8 Key label)<br>
AT+SLEEP (1 Light sleep, 2 Deep sleep no broadcast)<br>
AT+DEFAULT<br>
AT+INQ (0 2secs, 1 4secs, 2 6secs, 3 8secs, 4 16secs)<br>
AT+STOP<br>
AT+CONN (0-7)<br>
AT+CONA (Mac hex)<br>
AT+BAND (Mac hex)<br>
AT+CLRBAND<br>
AT+SRBAND<br>
AT+MESH (Hex data)<br>
AT+KEY(ioInput 1-5, target 000-ffff, ioOutput 1-5, 1 serialoutput 0 no serialoutput)<br>
AT+NETID(0000-ffff)<br>
AT+MADDR(0001-ffff)<br>
AT+MCLSS(0 router, 1 terminal)<br>
AT+FRIEND (Mac hex)<br>
AT+CLRFRIEND<br>
AT+FRTYPE(0 private, 1 open)<br>
AT+IBUUID(Hex UUID)<br>
AT+MAJOR(0000-ffff)<br>
AT+MINOR(0000-ffff)<br>
AT+IBSING(00-ff)<br>
AT+ENLOG(1 enable, 0 disable)<br>
AT+DATA(id 1-4, data)<br>
AT+CUIO(ioOutput 1-5)<br>
AT+TGIO(ioOutput 1-5)<br>
AT+FUNC<br>
AT+SUBTYPE(1 Key and Output associated, 0 Key and output NOT associated)<br>
AT+LEARN(1 on, 0 off)<br>
AT+LEAVAL(output pin, code sender INPUT pin number, code sender short address)<br>
AT+DEVCLSS(f1 one key binding function is turn on - in ROLE1mode support pressing PWRC pin to bind and connect to the strongest signal slave nearby)<br>
AT+KLABEL(label 0-9, ioNumber 1-5, Mac Addr)<br>
AT+KLTIME(label 0-9, overtime 0-9)<br>
AT+KLRSSI(label 0-9, sensitivity 00-99)<br>

- Active current IBEACON: average 2.47mA, max 5.65mA<br>
- Trans. current IBEACON -15dBM: average 4.36mA, max 7.23mA, dur 3.5ms<br>
- Trans. current IBEACON -0dBm: average 6.74mA, max 10.47mA, dur 3.7ms<br>
- Trans. current IBEACON 2dBM: average 7.00mA, max 11.63mA, dur 3.5ms<br>
- Trans. current IBEACON 3dBM: average 7.55mA, max 13.00mA, dur 3.5ms<br>
- Trans. current IBEACON 4dBM: average 8.64mA, max 14.80mA, dur 3.5ms<br>

- Sleeping current: average 1.92uA, max 2.82uA<br>
- Trans. current IBEACON -15dBM: average 2.6mA, max 7.00mA, dur 7.5ms<br>
- Trans. current IBEACON -0dBm: average 3.50mA, max 10.62mA, dur 7.8ms<br>
- Trans. current IBEACON 2dBM: average 3.57mA, max 11.80mA, dur 7.8ms<br>
- Trans. current IBEACON 3dBM: average 3.80mA, max 13.00mA, dur 7.8ms<br>
- Trans. current IBEACON 4dBM: average 4.0mA, max 15.00mA, dur 7.8ms<br>