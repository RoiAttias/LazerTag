#ifndef COMMS_HPP
#define COMMS_HPP

#include "Utilities/HyperMap.hpp"
#include "LazerTagPacket.hpp"

namespace Comms {
    const uint8_t version[3] = {SYSTEM_VERSION_MAJOR, SYSTEM_VERSION_MINOR, SYSTEM_VERSION_PATCH};
    uint8_t deviceType;

    HyperList<MacAddress> peers;
    HyperMap<MacAddress, uint8_t> peerTypes;

    bool init(uint8_t setDeviceType) {
        CHANNEL = channel; // Set the channel

        // Initialize ESP-NOW
        WiFi.mode(WIFI_STA); // Set the WiFi mode to station
        if (esp_now_init() != ESP_OK) return false; // Initialize ESP-NOW

        // Register the callbacks
        if (esp_now_register_recv_cb(onReceive) != ESP_OK) return false;

        if (addPeer(BROADCAST_ADDRESS) != NEXUS_ERROR_OK) return false; // Add the broadcast peer

        return true; // Initialization successful
    }

    void addPeer(const MacAddress &peer) {
        peers.addend(peer);
    }

    void loop() {
        Nexus::loop();
    }

    uint8_t available() {
        return Nexus::available();
    }

    void sendPacket(LazerTagPacket &packet, const MacAddress &destination) {
        uint8_t data[packet.size()]; // Create a buffer for the packet
        memcpy(data, &packet, packet.size()); // Copy the packet to the buffer
        esp_now_send(destination.addr, data, packet.size());
    }
}

#endif // COMMS_HPP