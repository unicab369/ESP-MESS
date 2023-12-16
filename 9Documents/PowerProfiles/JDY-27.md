# JDY-27

### Commands
default Baudrate: 9600. Line ending: \r\n <br>

- AT+RST<br>
- AT+VER<br>
- AT+RST<br>
- AT+BAUD<br>
- AT+NAME<br>
- AT+STAT<br>
- AT+DISC<br>
- AT+MAC<br>
- AT+DEFAULT<br>
- AT+POWR (can set but see no difference)<br>
- AT+ADVIN

### Observed Behaviors

1. Light Sleep (mode 1):
   - baacon interval 200ms (1): average current draw 50uA.
   - beacon interval 500ms (4): average current draw 21uA.
   - beacon interval 1000ms (7): average current draw 13uA.

2. Active (No Connection):
   - broadcast spike POWR8 average 1.52mA, max 4.80mA, duration ~5.5ms.
   - beacon interval 200ms: (1) average current draw 285uA.
   - beacon interval 500ms (4): average current draw 270uA.
   - beacon interval 1000ms (7): average current draw 256uA.

3. Active (With Connection):
   - average current draw 321uA.


1. The serial port baud rate is 9600 AT+BAUD4
2. Wake up AT+STARTEN0 on startup in sleep mode
3. Broadcast name JDY-27 AT+ NAMJDY-27
4. Broadcast interval 200MS AT+ADVIN1


1 AT test S/M
2 AT+VER Version number S/M JDDY-27-v1.94
3 AT+RST Soft resets S/M -
4 AT+HOSTEN Working Mode (Master/Slave) Configuring S/M 0 (slave)
5 AT+ENLOG Serial port output LOG switch S/M 1
6 AT+ALED indicator switch S/M 1
7 AT+SLEEP Sleep instruction S/M
8 AT+BAUD The serial port baud rate is set to S/M 4
9 AT+UARTMODE Serial port parity and stop bit configuration S/M 0,0
10 AT+STARTEN Power on sleep configuration S 0
11 The AT+ADVIN broadcast interval is set to S1
12 The AT+NETIN connection interval is set to S1
13 AT+POWR Transmit power configuration S/ M8
14 AT+MAME The broadcast name is set to SDY-27
15 AT+MAC The MAC address is S/M
16 AT+STAT Connection status Query S/M 00
17 AT+DISC Disconnects S/M
In 18 AT+INQ host mode, search for slave M
19 AT+SINQ host mode Actively stops searching for M
20 The MAC address is connected to M in AT+CONA host mode
Bind MAC Address M 000000000000 to the slave computer in 21 AT+BAND host mode
22 AT+DEFAULT Restore S/M to factory Settings

AT instruction specification
Special note: JDY-27 module MCU AT instruction need to add end character \r\n, serial tool send AT instruction do not need to add \r\n, check send carriage return equivalent to add \r\n
Mobile command

APP UUID
Service UUID: 0xFFE0 (Default service UUID is 0xFFE0)
Feature 1UUID: 0xFFE1(Used for transparent transmission of the default 0xFFE1 attribute notify and write)
Feature 2UUID: 0xFFE2(for IO control default 0xFFE2 attribute write)
