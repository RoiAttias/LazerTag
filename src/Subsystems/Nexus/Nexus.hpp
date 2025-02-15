#ifndef NEXUS_HPP
#define NEXUS_HPP

#include <Arduino.h>

#include <queue>

#include <esp_now.h>
#include <WiFi.h>

#include "Utilities/HyperList.hpp"
#include "MacAddress.hpp"


#define NEXUS_VERSION 0x01

#define NEXUS_PACKET_FLAG_SYN 0x01 // Synchronize - Establish a connection
#define NEXUS_PACKET_FLAG_ACK 0x02 // Acknowledge - Acknowledge a received packet
#define NEXUS_PACKET_FLAG_FIN 0x04 // Finish - Close a connection
#define NEXUS_PACKET_FLAG_RST 0x08 // Reset - Reset a connection

#define NEXUS_PACKET_FLAG_PSH 0x10 // Push - Send data immediately
#define NEXUS_PACKET_FLAG_CHK 0x80 // Checksum - Verify the integrity of the packet

struct NexusPacket {
    // Header
    uint8_t version; // 1 byte
    uint16_t sequenceNum; // 2 bytes
    uint16_t acknowledgeNum; // 2 bytes
    uint16_t flags; // 2 bytes = 16 bits
    uint16_t checksum; // 2 bytes
    uint8_t length; // 1 byte

    // Header size = 1 + 2 + 2 + 2 + 2 + 1 = 10 bytes

    uint8_t payload[ESP_NOW_MAX_DATA_LEN - 10];
    
    // Total packet size = 250 bytes

    NexusPacket(uint16_t seqNum, uint16_t ackNum, uint16_t flags, uint8_t len, uint8_t *payload) {
        version = NEXUS_VERSION;
        sequenceNum = seqNum;
        acknowledgeNum = ackNum;
        this->flags = flags;
        length = len;
        memcpy(this->payload, payload, len);
    }

    NexusPacket() {
        version = NEXUS_VERSION;
        sequenceNum = 0;
        acknowledgeNum = 0;
        flags = 0;
        length = 0;
    }

    size_t size() const {
        return length + 10;
    }

    /**
     * @brief Verify the integrity of the packet.
     * @return True if the packet is valid, false otherwise.
     */
    bool verifyChecksum() {
        return countOnes() == checksum;
    }

    /**
     * @brief Count the number of bits set of the payload.  
     * @return The number of bits set in the payload.
     */
    uint16_t countOnes() {
        uint16_t countTotal = 0;
        uint8_t countByte = 0, byteMask = 0x01;
        for (size_t i = 0; i < length; i++) {
            countByte = 0;
            while (byteMask) {
                countByte += (payload[i] & byteMask) ? 1 : 0;
                byteMask <<= 1;
            }
            countTotal += countByte;
        }
        return countTotal;
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

/*
struct NexusPeer {
    MacAddress macAddress;

    uint16_t lastSequenceNum = 0;
    uint16_t lastAcknowledgeNum = 0;

    NexusPeer(const MacAddress &mac) : macAddress(mac) {}

    uint8_t *getAddress() {
        return macAddress.getAddress();
    }
};
*/

enum NexusPeerEventType : uint8_t {
    NEXUS_PEER_EVENT_SYNCRONIZE_INIT,
    NEXUS_PEER_EVENT_SYNCRONIZE_ACK,
    NEXUS_PEER_EVENT_SYNCRONIZE_INIT_ACK,
    NEXUS_PEER_EVENT_FINISH_INIT,
    NEXUS_PEER_EVENT_FINISH_ACK,
    NEXUS_PEER_EVENT_RESET_INIT,
    NEXUS_PEER_EVENT_RESET_ACK,
};

struct NexusPeerEvent {
    MacAddress addr;
    NexusPeerEventType eventType;
    uint16_t sequenceNum;
};

/**
  * @brief Namespace for ESP-NOW communication;
  * @details This namespace provides functions for sending and receiving packets
  *          using the ESP-NOW protocol.
  * @warning This namespace is not thread-safe and should not be accessed concurrently from multiple threads.
  */
namespace Nexus {
    // Constants and Variables
    const size_t MAX_PEERS = 20;
    
    int CHANNEL = 0;

    // Addresses
    const uint8_t BROADCAST_ADDRESS_ARRAY[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const MacAddress BROADCAST_ADDRESS(BROADCAST_ADDRESS_ARRAY);
    MacAddress THIS_ADDRESS;

    // Lists
    HyperList<MacAddress> peers;
    PacketBuffer<NexusPacket> packetBuffer(32);
    std::queue<NexusPeerEvent> pendingPeersEvents;

    // ESP-NOW Callbacks
    void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
        MacAddress mac_addr(mac);
        NexusPacket packet;
        memcpy(&packet, data, len);

        // Check the version
        if (packet.version != NEXUS_VERSION) return;
        
        switch (packet.flags)
        {
        case NEXUS_PACKET_FLAG_SYN:
            SynchronizeAcceptor(mac_addr, packet);
            break;
        
        case NEXUS_PACKET_FLAG_SYN | NEXUS_PACKET_FLAG_ACK:
            if (SynchronizeInitiatorAck(mac_addr, packet))
            {
                addPeer(mac_addr);
            }
            break;

        case NEXUS_PACKET_FLAG_FIN:
            if (FinishAcceptor(mac_addr, packet))
            {
                removePeer(mac_addr);
            }
            break;

        case NEXUS_PACKET_FLAG_FIN | NEXUS_PACKET_FLAG_ACK:
            if (FinishAcceptor(mac_addr, packet))
            {
                removePeer(mac_addr);
            }
            break;
            
        
        case NEXUS_PACKET_FLAG_RST:
            ResetAcceptor(mac_addr, packet);
            break;
        }

        // Check the checksum
        if ((packet.flags & NEXUS_PACKET_FLAG_CHK) && (!packet.verifyChecksum()))
        {
            // Checksum failed
            pendingPeersEvents.push({macAddr, NEXUS_PEER_EVENT_RESET_INIT});
            return;
        }


        
        packetBuffer.enqueue(packet);
    }

    void onSend(const uint8_t *mac_addr, esp_now_send_status_t status) {
        if (status == ESP_NOW_SEND_SUCCESS) {
            // Packet sent successfully
        } else {
            // Packet failed to send
            // Reset the peer
            pendingPeersEvents.push({MacAddress(mac_addr), NEXUS_PEER_EVENT_RESET});
        }
    }

    void addPeer(const MacAddress &peer) {
        if (peers.contains(peer)) return;
        if (peers.size() < MAX_PEERS) {
            peers.addend(peer);
            esp_now_peer_info_t peerInfo = {};
            peer.toBuffer(peerInfo.peer_addr);
            peerInfo.channel = CHANNEL;
            peerInfo.encrypt = false;
            esp_now_add_peer(&peerInfo);
        }
    }

    void removePeer(const MacAddress &peer) {
        peers.removeValue(peer);
        esp_now_del_peer(peer.addr);
    }

    bool begin(int channel = 0) {
        // Set the channel
        CHANNEL = channel;
        
        // Get the MAC address of the ESP32
        if(esp_base_mac_addr_get(THIS_ADDRESS.addr) != ESP_OK) return false;
        
        // Set the ESP-NOW mode to station
        WiFi.mode(WIFI_STA);

        // Initialize ESP-NOW
        if (esp_now_init() != ESP_OK) return false;
        
        // Register the callbacks
        esp_now_register_recv_cb(onReceive);
        esp_now_register_send_cb(onSend);
    }

    bool sendPacket(const NexusPacket &packet, const MacAddress &destination = BROADCAST_ADDRESS) {
        return esp_now_send(destination, (uint8_t *)&packet, packet.size) == ESP_OK;
    }

    bool receivePacket(NexusPacket &packet) {
        return packetBuffer.dequeue(packet);
    }

    void autoPeerInitiator(const MacAddress &target) {
        addPeer(target);
    }

    void autoPeerAcceptor(const MacAddress &peer) {
        addPeer(peer);
    }


    // Peer Events
    bool SynchronizeInitiator(const MacAddress &target) {
        NexusPacket packet(random(1,256) & 0xFF, 0, NEXUS_PACKET_FLAG_SYN, 0, nullptr);
        if (sendPacket(packet, target) == false) return false;

        pendingPeersEvents.push({target, NEXUS_PEER_EVENT_SYNCRONIZE_ACK});
        return true;
    }

    bool SynchronizeAcceptor(const MacAddress &target, NexusPacket packet) {
        packet.acknowledgeNum = packet.sequenceNum + 1;
        packet.sequenceNum = acknowledgeNum + (random(100,200) & 0xFF);
        packet.flags = NEXUS_PACKET_FLAG_SYN | NEXUS_PACKET_FLAG_ACK;
        if (sendPacket(packet, target) == false) return false;

        pendingPeersEvents.push({peer, NEXUS_PEER_EVENT_SYNCRONIZE_INIT_ACK});
        return true;
    }

    bool SynchronizeInitiatorAck(const MacAddress &target, NexusPacket packet) {
        packet.acknowledgeNum = packet.sequenceNum + 1;
        packet.sequenceNum = 0;
        packet.flags = NEXUS_PACKET_FLAG_ACK;
        if (sendPacket(packet) == false) return false;

        addPeer(target);
        return true;
    }

    bool FinishInitiator(const MacAddress &target) {
        NexusPacket packet(random(1,256) & 0xFF, 0, NEXUS_PACKET_FLAG_FIN, 0, nullptr);
        if (sendPacket(packet) == false) return false;

        pendingPeersEvents.push({target, NEXUS_PEER_EVENT_FINISH_ACK});
    }

    bool FinishAcceptor(const MacAddress &target, NexusPacket packet) {
        packet.acknowledgeNum = packet.sequenceNum + 1;
        packet.sequenceNum = 0;
        packet.flags = NEXUS_PACKET_FLAG_FIN | NEXUS_PACKET_FLAG_ACK;
        if (sendPacket(packet) == false) return false;

        return true;
    }

    bool ResetInitiator(const MacAddress &target) {
        NexusPacket packet(random(1,256) & 0xFF, 0, NEXUS_PACKET_FLAG_RST, 0, nullptr);
        if (sendPacket(packet) == false) return false;

        pendingPeersEvents.push({target, NEXUS_PEER_EVENT_RESET_ACK});
    }

    bool ResetAcceptor(const MacAddress &target, NexusPacket packet) {
        packet.acknowledgeNum = packet.sequenceNum + 1;
        packet.sequenceNum = 0;
        packet.flags = NEXUS_PACKET_FLAG_RST | NEXUS_PACKET_FLAG_ACK;
        if (sendPacket(packet) == false) return false;

        return true;
    }
}

#endif // NEXUS_HPP