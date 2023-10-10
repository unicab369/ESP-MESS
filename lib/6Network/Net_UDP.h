#include <WiFiUdp.h>

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
const int SECS_IN_HOUR = 3600;
static const char ntpServerName[] = "us.pool.ntp.org";

class Net_UDP {
    WiFiUDP udp;
    
    // const int timeZone = -5;  // Eastern Standard Time (USA)
    const int timeZone = -4;  // Eastern Daylight Time (USA)
    //const int timeZone = -8;  // Pacific Standard Time (USA)
    //const int timeZone = -7;  // Pacific Daylight Time (USA)
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
    unsigned int localPort = 1234;  

    void sendPacket(IPAddress &address, byte *data, size_t size, int port) {
        if (callback == nullptr && timeRequestCb == nullptr) { return; }
        udp.beginPacket(address, port);
        udp.write(data, size);
        udp.endPacket();
    }

    ReceivePacketCb *callback;
    std::function<void(time_t*)> *timeRequestCb;

    public:
        void reload(ReceivePacketCb *cb) {
            AppPrint("[UDP]", __func__);
            udp.begin(localPort);
            callback = cb;
            timeRequestCb = NULL;
        }    

        void sendPacket(IPAddress &address, DataPacket *packet, int port) {
            AppPrint("[UDP]", __func__);
            sendPacket(address, (uint8_t*)packet, sizeof(DataPacket), port);
        }

        void broadcast(DataPacket *packet, int port = 1234) {
            IPAddress broadcast_ip(255, 255, 255, 255);
            sendPacket(broadcast_ip, packet, port);
        }

        void requestTime(std::function<void(time_t*)> *cb) {
            udp.begin(localPort);
            timeRequestCb = cb;
            callback = NULL;

            IPAddress ntpServerIP; // NTP server's ip address
            WiFi.hostByName(ntpServerName, ntpServerIP);
            Serial.print("\n[UDP]"); AppPrint(__func__, ntpServerName);

            // set all bytes in the buffer to 0
            memset(packetBuffer, 0, NTP_PACKET_SIZE);
            // Initialize values needed to form NTP request (see URL above for details on the packets)
            packetBuffer[0] = 0b11100011;   // LI, Version, Mode
            packetBuffer[1] = 0;     // Stratum, or type of clock
            packetBuffer[2] = 6;     // Polling Interval
            packetBuffer[3] = 0xEC;  // Peer Clock Precision
            // 8 bytes of zero for Root Delay & Root Dispersion
            packetBuffer[12] = 49;
            packetBuffer[13] = 0x4E;
            packetBuffer[14] = 49;
            packetBuffer[15] = 52;

            sendPacket(ntpServerIP, packetBuffer, NTP_PACKET_SIZE, 123);
        }

        void run() {
            if (callback) {
                int readLen = udp.parsePacket();
                if (!readLen) { return; }
                AppPrint("\n[UDP] Received");

                ReceivePacket packet;
                udp.read((char*)&packet.packetData, sizeof(DataPacket));
                (*callback)(&packet);

            } else if (timeRequestCb) {
                int readLen = udp.parsePacket();
                if (readLen < NTP_PACKET_SIZE) { return; }

                udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
                unsigned long secsSince1900;
                // convert four bytes starting at location 40 to a long integer
                secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
                secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
                secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
                secsSince1900 |= (unsigned long)packetBuffer[43];
                time_t time = secsSince1900 - 2208988800UL + timeZone * SECS_IN_HOUR;
                (*timeRequestCb)(&time);
            }
        }
};
