#include "PacketAnalyzer.h"
#define MAX_PACKET_LENGTH 2000
PacketAnalyzer packetAnalyzer;

#ifdef ESP32
    void onSniffCallback(void* buff, wifi_promiscuous_pkt_type_t type) {
        wifi_promiscuous_pkt_t* packet = (wifi_promiscuous_pkt_t*)buff;
        if (type == WIFI_PKT_MISC) return;          // wrong packet type
        
        uint32_t packetLength = packet->rx_ctrl.sig_len;
        if (packetLength > MAX_PACKET_LENGTH) return;  // packet too long
        if (type == WIFI_PKT_MGMT) {
            packetAnalyzer.checkDeauth(packet->payload);
            packetLength -= 4;
        }

        int8_t rssi = packet->rx_ctrl.rssi;
        packetAnalyzer.update(rssi);

        uint8_t targetMac[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
        PacketInfo* pkInfo = (PacketInfo*)packet->payload;
        // packetTracker.check(pkInfo->addr2, type);

        if (pkInfo->checkAddr(targetMac) == false) return;
        pkInfo->print();
        AppPrint("Received from channel", packet->rx_ctrl.channel);

        AppPrint("Print Payload");
        AppPrintHex(packet->payload, packetLength);

        if (pkInfo->data[0] == 0x77) {
            AppPrint("IM HERE zzz = ", pkInfo->timestamp);
            uint8_t addr1[6] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
            uint8_t addr2[6] = { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC };
            uint8_t addr3[6] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

            PacketInfo packet = PacketInfo::make(addr1, addr2, addr3);
            packet.data[0] = 0x88;
            packet.timestamp = pkInfo->timestamp;

            esp_wifi_80211_tx(WIFI_IF_AP, &packet, sizeof(PacketInfo) , false);

        } else if (pkInfo->data[0] == 0x88) {
            AppPrint("TIME DIF", millis() - pkInfo->timestamp);
        }

        // switch (type) {
        //     case WIFI_PKT_MGMT: AppPrint("\nWIFI_PKT_MGMT"); break;
        //     case WIFI_PKT_CTRL: AppPrint("\nWIFI_PKT_CTRL"); break;
        //     case WIFI_PKT_DATA: AppPrint("\nWIFI_PKT_DATA"); break;
        //     case WIFI_PKT_MISC: AppPrint("\nWIFI_PKT_MISC"); break;
        // }
    }
#else
    void onSniffCallback(uint8_t *buf, uint16_t len) {
        if (len > MAX_PACKET_LENGTH) return;  // packet too long

    }
#endif

byte BROADCAST_ADDR[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

#ifdef ESP32
    #include <esp_now.h>
#else
    #include <espnow.h>
#endif

class Interface_Net {
    public:
        virtual uint8_t* getMac() { 
            uint8_t mac[6] = {0};
            return mac; 
        }

        virtual void sendData(void* raw, size_t len) {}
};


class Net_EspNow: public Interface_Net {            
	void addPeer(byte *peer, uint8_t channel) {
        AppPrint("[EspN]", __func__);
		#ifdef ESP32
			esp_now_peer_info_t peerInfo;
			
			memcpy(peerInfo.peer_addr, peer, 6);
			peerInfo.ifidx = WIFI_IF_STA;
			peerInfo.channel = channel;
			peerInfo.encrypt = false;
			// peerInfo.encrypt = useKey;
			if (esp_now_add_peer(&peerInfo) != 0) {
				Serial.println("[EspNow] Failed to add peer");
			}
		#else 
			esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
			esp_now_add_peer(peer, ESP_NOW_ROLE_SLAVE, channel, NULL, 0);
		#endif			
	}

    protected:
        bool isLoaded = false;
        
        void begin(int32_t channel, esp_now_recv_cb_t cb) {
            AppPrint("[EspN]", __func__);
            // esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
            // wifi_set_channel(channel);

            if (esp_now_init() != 0) {
				Serial.print("[EspN] Init failed"); Serial.println(channel);
				return;                
            }

            AppPrint("[EspN] mac", WiFi.macAddress());
            AppPrint("[EspN] Init ch", channel);
            esp_now_register_recv_cb(cb);

            // clear all the peers
            #ifdef ESP32
                    // turn on promiscuous to capture everything
                esp_wifi_set_promiscuous(true);
                esp_wifi_set_promiscuous_rx_cb(&onSniffCallback);

                esp_now_peer_info_t peerInfo;
                while (esp_now_fetch_peer(true, &peerInfo) == ESP_OK) {
                    esp_now_del_peer(peerInfo.peer_addr);
                }
            #else
                wifi_promiscuous_enable(1);
                wifi_set_promiscuous_rx_cb(onSniffCallback);
            #endif
            
            addPeer(BROADCAST_ADDR, channel);
            WiFi.macAddress(mac);
            AppPrint("[EspN] curr ch", WiFi.channel());

            isLoaded = true;
        }

    public: 
        uint8_t mac[6];

        uint8_t* getMac() override { return mac; }

        void sendData(void* raw, size_t len) override {
            if (!isLoaded) { return; }
            // AppPrint("\n[EspN]", __func__);
            // AppPrintHex(raw, len);
            
            esp_now_send(BROADCAST_ADDR, (uint8_t*) raw, len);
        }

        void sendCustomPacket() {
            AppPrint("[EspN]", __func__);
            uint8_t addr1[6] = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA };
            uint8_t addr2[6] = { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC };
            uint8_t addr3[6] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

            PacketInfo packet = PacketInfo::make(addr1, addr2, addr3);
            packet.data[0] = 0x77;
            AppPrint("MyPacket = ", packet.timestamp);
            AppPrintHex(&packet, sizeof(PacketInfo));

            #ifdef ESP32
                esp_wifi_80211_tx(WIFI_IF_AP, &packet, sizeof(packet) , false);
            #else
                uint8_t* pk = (uint8_t*)&packet;
                wifi_send_pkt_freedom(pk, sizeof(packet), false);
            #endif
        }
};

#define MAX_MSG_QUEUE 5
AppQueue<ReceivePacket2, MAX_MSG_QUEUE> msgQueue2;

#ifdef ESP32
    void receive_callback(const uint8_t *sender, const uint8_t *data, int data_len) {
        // AppPrint("[EspN] Received");
        ReceivePacket2 receiv_packet = ReceivePacket2::make(sender, data);
        msgQueue2.sendQueue(&receiv_packet);
    }
#else
    void receive_callback(uint8_t *sender, uint8_t *data, uint8_t len) {
        // Serial.print("\nReceiv from "); PrintBytes(sender, 6, ':');
        ReceivePacket2 receiv_packet = ReceivePacket2::make(sender, data);
        msgQueue2.sendQueue(&receiv_packet);
    }
#endif

class Serv_EspNow: public Net_EspNow {
    uint32_t selectedChannel = 1;

    void reload() {
        begin(selectedChannel, receive_callback);
    }

    public:
        std::function<void(ReceivePacket2*)> callback = [](ReceivePacket2*){};
        
        void rollChannel() {
            selectedChannel++;
            if (selectedChannel>13) { selectedChannel = 1; }
            reload();
        }

        void changeChannel(uint32_t channel) {
            selectedChannel = channel;
            reload();
        }

        void setup(uint32_t channel) {
            AppPrintSeparator("[EspN]", __func__);
            selectedChannel = channel;
            reload();
        }

        void run() {
            ReceivePacket2 item;
            while (msgQueue2.getQueue(&item)) {
                callback(&item);
            }
        }
};
