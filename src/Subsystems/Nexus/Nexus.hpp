#ifndef NEXUS_HPP
#define NEXUS_HPP

#include <Arduino.h>
#include "MacAddress.h"
#include <vector>
#include <queue>
#include <esp_now.h>

#define NEXUS_PACKET_FLAG_SYN 0x01
#define NEXUS_PACKET_FLAG_ACK 0x02
#define NEXUS_PACKET_FLAG_FIN 0x04
#define NEXUS_PACKET_FLAG_RST 0x08

#define NEXUS_PACKET_FLAG_WIN 0x10
#define NEXUS_PACKET_FLAG_PSH 0x20
#define NEXUS_PACKET_FLAG_URG 0x40
#define NEXUS_PACKET_FLAG_CHK 0x80

struct NexusPacket {
    uint16_t sequenceNum;
    uint16_t acknowledgeNum;
    uint16_t flags;
    MacAddress endDevice;
    uint8_t length;
    uint8_t payload[240];

    NexusPacket(type, seqNum, endDev, len) : type(type), sequenceNum(seqNum), endDevice(endDev), length(len) {}

    bool varifyParrity() {
        return countParrity() % 2 == 0;
    }

    uint16_t countParrity() {
        uint16_t count = 0;
        for (size_t i = 0; i < length; i++) {
            count += __builtin_popcount(payload[i]); // Count the number of set bits
        }
        return count;
    }
};

/**
 * Class template for a packet buffer.
 * @tparam T Type of packet to be stored in the buffer.
 * @tparam maxSize Maximum number of packets that can be stored in the buffer.
 * @warning The buffer is not thread-safe and should not be accessed concurrently from multiple threads.
 */
template <typename T>
class PacketBuffer {
private:
    std::queue<T> packetQueue;
    size_t maxSize;

public:
    PacketBuffer(size_t size) : maxSize(size) {}

    bool enqueue(const T& packet) {
        if (packetQueue.size() >= maxSize) return false;
        packetQueue.push(packet);
        return true;
    }

    bool dequeue(T& packet) {
        if (packetQueue.empty()) return false;
        packet = packetQueue.front();
        packetQueue.pop();
        return true;
    }

    bool isEmpty() const {
        return packetQueue.empty();
    }

    size_t size() const {
        return packetQueue.size();
    }

    void clear() {
        while (!packetQueue.empty()) {
            packetQueue.pop();
        }
    }
};

struct NexusPeer {
    MacAddress macAddress;

    uint16_t lastSequenceNum = 0;


    NexusPeer(const MacAddress &mac) : macAddress(mac) {}
}

/**
  * @brief Namespace for ESP-NOW communication;
  * @details This namespace provides functions for sending and receiving packets
  *          using the ESP-NOW protocol.
  * @warning This namespace is not thread-safe and should not be accessed concurrently from multiple threads.
  */
namespace EspNowComm {
    static const size_t MAX_PEERS = 20;
    static const int channel = 0;
    static std::vector<MacAddress> peers;
    static PacketBuffer packetBuffer<NexusPacket>(32);

    void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
        EspNowPacket packet;
        memcpy(&packet, data, len);
        packetBuffer.enqueue(packet);
        autoPeerAcceptor(MacAddress(mac));
    }

    void onSend(const uint8_t *mac_addr, esp_now_send_status_t status) {
        autoPeerInitiator(MacAddress(mac_addr));
    }

    void addPeer(const MacAddress &peer) {
        for (const auto& p : peers) {
            if (p == peer) return;
        }
        if (peers.size() < MAX_PEERS) {
            peers.push_back(peer);
            esp_now_peer_info_t peerInfo = {};
            memcpy(peerInfo.peer_addr, peer.getAddress(), 6);
            peerInfo.channel = channel;
            peerInfo.encrypt = false;
            esp_now_add_peer(&peerInfo);
        }
    }

    void begin(int channel = 0) {
        if (esp_now_init() != ESP_OK) {
            Serial.println("Error initializing ESP-NOW");
            return;
        }
        esp_now_register_recv_cb(onReceive);
        esp_now_register_send_cb(onSend);
    }

    bool sendPacket(const EspNowPacket &packet) {
        return esp_now_send(packet.receiver.getAddress(), (uint8_t *)&packet, sizeof(packet)) == ESP_OK;
    }

    bool receivePacket(EspNowPacket &packet) {
        return packetBuffer.dequeue(packet);
    }

    void autoPeerInitiator(const MacAddress &target) {
        addPeer(target);
    }

    void autoPeerAcceptor(const MacAddress &peer) {
        addPeer(peer);
    }
}

#endif // NEXUS_HPP