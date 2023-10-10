// /**< If g_mconfig_scan_queue is full, delete the front item */
// if (!uxQueueSpacesAvailable(g_mconfig_scan_queue)) {
//     xQueueReceive(g_mconfig_scan_queue, &scan_info, 0);
// }

// scan_info.rssi = promiscuous_pkt->rx_ctrl.rssi;
// memcpy(scan_info.bssid, payload + 10, MWIFI_ADDR_LEN);
// xQueueSend(g_mconfig_scan_queue, &scan_info, 0);

#define MAX_X 96      //128
#define MAX_Y 51      //51

// resources:
// https://hackmag.com/security/esp32-sniffer/
// https://howiwifi.com/2020/07/13/802-11-frame-types-and-formats/
// https://en.wikipedia.org/wiki/802.11_Frame_Types
// https://github.com/spacehuhn/PacketMonitor32/blob/master/PacketMonitor32.ino

struct PacketAnalyzer {
    uint32_t channel = 0;
    uint32_t deauths = 0; 
    uint32_t packetCounter = 0;
    int rssiSum;
    uint32_t pkts[MAX_X]; // here the packets per second will be saved

    uint32_t getNextChannel() {
        uint32_t output = channel++;
        return (output<1) ? 1 : (output>13) ? 13 : output;
    }

    double getMultiplicator() {
        uint32_t maxVal = 1;
        for (int i = 0; i < MAX_X; i++) {
            if (pkts[i] > maxVal) maxVal = pkts[i];
        }
        if (maxVal > MAX_Y) return (double)MAX_Y / (double)maxVal;
        return 1;
    }

    String log() {
        int rssi;
        if (pkts[MAX_X - 1] > 0) rssi = rssiSum / (int)pkts[MAX_X - 1];
        else rssi = rssiSum;
        return "ch" + String(channel) + " " + (String)rssi + " Pk " + (String)packetCounter + " [" + deauths + "]";
    }

    void checkDeauth(uint8_t *payload) {
        if ((payload[0] != 0xA0) && (payload[0] != 0xC0)) return;
        deauths++;
    }

    void update(int8_t rssi) {
        packetCounter++;
        rssiSum += rssi;
    }

    void clear() {
        deauths = 0;
        rssiSum = 0;
        packetCounter = 0;
    }

    void draw(Disp_SSD13062 *disp) {
        disp->printline(log(), 0);
        disp->drawLine(0, 63 - MAX_Y, MAX_X, 63 - MAX_Y);

        int len;
        double multiplicator = getMultiplicator();
        pkts[MAX_X - 1] = packetCounter;

        for (int i = 0; i < MAX_X; i++) {
            len = pkts[i] * multiplicator;
            disp->drawLine(i, 63, i, 64 - MAX_Y, BLACK);                    // LINE ERASE
            disp->drawLine(i, 63, i, 63 - (len > MAX_Y ? MAX_Y : len));     // LINE DRAW
            if (i < MAX_X - 1) pkts[i] = pkts[i + 1];       
        }

        clear();
    }
};

struct PacketInfo {
    uint16_t protocolVersion = 0x0009;      // 0x0900 ?? 0x0009
    uint16_t frameControlType = 0x1010;
    uint8_t addr1[6], addr2[6], addr3[6];
    uint32_t timestamp;
    char data[32] = "";
    
    static PacketInfo make(uint8_t* _addr1, uint8_t* _addr2, uint8_t* _addr3) {
        PacketInfo output;
        output.timestamp = millis();
        memcpy(output.addr1, _addr1, sizeof(output.addr1));
        memcpy(output.addr2, _addr2, sizeof(output.addr1));
        memcpy(output.addr3, _addr3, sizeof(output.addr1));
        return output;
    }

    bool checkAddr(uint8_t *targetMac) {
        return memcmp(addr1, targetMac, sizeof(addr1)) == 0;
    }

    void print() {
        printf("\n\nControlFrame: 0x%04X 0x%04X", protocolVersion, frameControlType);
        char hexStr[sizeof(addr1)] = "";
        AppPrint("Addr1"); AppPrintHex(addr1, sizeof(addr1));
        AppPrint("Addr2"); AppPrintHex(addr2, sizeof(addr2));
        AppPrint("Addr3"); AppPrintHex(addr3, sizeof(addr3));
    }
};

uint8_t INVALID_MAC[6] = { 0 };

struct PacketCounter {
    uint8_t addr[6] = { 0 };
    uint32_t counter;
    uint8_t type;

    bool isValid() { 
        return memcmp(addr, INVALID_MAC, sizeof(addr)) != 0; 
    }

    bool check(uint8_t *targetMac) {
        if (memcmp(addr, targetMac, sizeof(addr)) != 0) return false;
        counter++;
        return true;
    }

    void add(uint8_t *targetMac, uint8_t setType) {
        memcpy(addr, targetMac, sizeof(addr));
        counter=1;
        type = setType;
    }

    void print() {
        // char hexStr[18] = "";
        // AppMacPrint(hexStr, addr);
        // AppPrint("[PktCount]", String(hexStr) + " " + String(counter));
    }
};

#define MAX_PACKET_COUNTER 20

struct PacketTracker {
    PacketCounter pktCounter[MAX_PACKET_COUNTER];
    uint16_t currIndex = 0;

    void clear() {
        AppPrint("\n[PkTracker]", __func__);
        // currIndex = 0;
        // PacketCounter clearedCounter;
        // for (int i=0; i<MAX_PACKET_COUNTER; i++) {
        //     pktCounter[i] = clearedCounter;
        // }
    }

    void check(uint8_t *targetMac, uint8_t type) {
        if (currIndex>MAX_PACKET_COUNTER-1) {
            AppPrint("\nPacket Counter Full");
            return;
        }

        for (int i=0; i<currIndex; i++) {
            PacketCounter target = pktCounter[i];
            if (target.check(targetMac)) return;
        }

        pktCounter[currIndex].add(targetMac, type);
        currIndex++;
    }

    void print() {
        for (int i=0; i<currIndex; i++) {
            PacketCounter target = pktCounter[i];
            if (target.isValid()) target.print();
        }
        clear();
    }
};

PacketTracker packetTracker;