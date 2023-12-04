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
