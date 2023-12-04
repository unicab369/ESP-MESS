# JDY-23

### Links
https://fcc.report/FCC-ID/2AXM8-JDY-23/4936741.pdf

### Commands
default Baudrate: 9600. Line ending: \r\n

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
   - beacon interval 100ms: average current draw 9.10uA.
   - beacon interval 1000ms: average current draw 8.95uA.

2. Light Sleep (mode 1):
   - many short spikes, broadcast spike average 2.95mA, max 10mA, duration ~9.5ms.
   - beacon interval 200ms: average current draw 135uA.
   - beacon interval 500ms: average current draw 60uA.
   - beacon interval 1000ms: average current draw 35uA.

3. Active (No Connection):
   - broadcast spike average 3.8mA, max 10mA, duration ~6.4ms.
   - beacon interval 200ms: average current draw 813uA.
   - beacon interval 500ms: average current draw 753uA.
   - beacon interval 1000ms: average current draw 732uA.

4. Active (with Connection):
   - broadcast spike average 9.40mA, max 15mA, duration ~4.3ms.
   - beacon interval 100ms: average current draw 7.80mA.
   
3. Others:
   - required reset for changes to take place
   - only 2 features UUID usable, one fir transparent transmission and the battery level
   - the chip I have the firmware doesn't allow read and write on the IOs

### Figures
![Alt text](images/JDY-23_Sleep2_NoBroadcast.png)
Figure 1. Sleep Mode2 no broadcast.

![Alt text](images/JDY-23_Sleep1.png)
Figure 2. Sleep Mode1.

![Alt text](images/JDY-23_Signal_SleepMode.png)
Figure 3. Beacon signal in sleep mode.

![Alt text](images/JDY-23_Connected.png)
Figure 4. Connected state.