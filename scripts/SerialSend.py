import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
serialObj = serial.Serial()
portList = []

TEST_PORT = "COM5"
# TEST_PORT = None

if (TEST_PORT):
    print("selected Port: ", TEST_PORT)
    serialObj.port = TEST_PORT
else:
    for port in ports:
        portList.append(str(port))
        print(str(port))

    val = input("select Port: COM")

    for x in range(0, len(portList)):
        if portList[x].startswith("COM" + str(val)):
            portVar = "COM" + str(val)
            print()
            print(portList[x])
    serialObj.port = portVar

serialObj.baudrate = 115200
serialObj.open()

receiveStr = ""

while True:
    # if serialObj.in_waiting:
    #     packet = serialObj.readline()
    #     print(packet.decode('utf'))
    # else:
    command = input("Send Command: ")
    serialObj.write(command.encode('utf-8'))
    packet = serialObj.readline()
    print(packet.decode('utf'))

    if command == 'exit':
        exit()