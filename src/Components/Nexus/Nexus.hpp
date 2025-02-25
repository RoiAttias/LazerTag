#ifndef NEXUS_HPP
#define NEXUS_HPP

#include <Arduino.h> // Include the Arduino library
#include <queue> // Include the queue library
#include <WiFi.h> // Include the WiFi library
#include <esp_wifi.h> // Include the ESP WiFi library
#include <esp_now.h> // Include the ESP-NOW library

#include "Utilities/MacAddress.hpp" // Include the MacAddress utility
#include "Utilities/PacketBuffer.hpp" // Include the PacketBuffer utility
#include "Utilities/HyperList.hpp" // Include the HyperList utility
#include "Utilities/HyperMap.hpp" // Include the HyperMap utility

// Define constants for Nexus
#define NEXUS_TIMEOUT 1000 // Timeout for Nexus operations
#define NEXUS_SCAN_INTERVAL 5000 // Interval for scanning devices
#define NEXUS_BUFFER_SIZE 32 // Buffer size for packets
#define NEXUS_HEADER_SIZE 7 // Header size for packets
#define NEXUS_MAX_PAYLOAD_SIZE (ESP_NOW_MAX_DATA_LEN - NEXUS_HEADER_SIZE) // Maximum payload size

#define NEXUS_VERSION 0x01 // Version of Nexus

// Define flags for Nexus packets
#define NEXUS_FLAG_NONE 0x00 // No flags - Data packet
#define NEXUS_FLAG_SYN 0x01 // Synchronize - Establish a connection
#define NEXUS_FLAG_ACK 0x02 // Acknowledge - Acknowledge a received packet
#define NEXUS_FLAG_FIN 0x04 // Finish - Close a connection
#define NEXUS_FLAG_RST 0x08 // Reset - Reset a connection
#define NEXUS_FLAG_SCN 0x10 // Scan - Scan for nearby devices

// Define error codes for Nexus
enum NexusErrors : uint8_t {
    NEXUS_ERROR_OK, // No error
    NEXUS_ERROR_UNKNOWN, // Unknown error
    NEXUS_ERROR_TIMEOUT, // Timeout error
    NEXUS_ERROR_BUFFER_FULL, // Buffer full error
    NEXUS_ERROR_PEER_REFUSED, // Peer refused error
    NEXUS_ERROR_PEER_FULL, // Peer full error
    NEXUS_ERROR_PEER_NOT_FOUND, // Peer not found error
    NEXUS_ERROR_PEER_ALREADY_EXISTS, // Peer already exists error
    NEXUS_ERROR_INVALID_VERSION, // Invalid version error
    NEXUS_ERROR_INVALID_FLAG, // Invalid flag error
    NEXUS_ERROR_INVALID_LENGTH // Invalid length error
};

// Define the NexusPacket structure
struct __attribute__((packed)) NexusPacket {
    uint8_t version; // 1 byte
    uint16_t sequenceNum; // 2 bytes
    uint16_t flags; // 2 bytes
    uint8_t error; // 1 byte
    uint8_t length; // 1 byte

    uint8_t payload[NEXUS_MAX_PAYLOAD_SIZE]; // 243 bytes

    // Constructor for NexusPacket
    NexusPacket(uint16_t sequenceNum, uint16_t flags, uint8_t error, uint8_t length, const uint8_t *payload)
        : version(NEXUS_VERSION), sequenceNum(sequenceNum), flags(flags), error(error), length(length) {
        if (payload != nullptr) {
            memcpy(this->payload, payload, length); // Copy the payload
        }
    }

    // Function to get the size of the packet
    size_t size() const {
        return length + NEXUS_HEADER_SIZE; // Return the total size of the packet
    }

    String toString() const {
        String str = "NexusPacket {\n";
        str += "Version: " + String(version) + "\n";
        str += "SequenceNum: " + String(sequenceNum) + "\n";
        str += "Flags: " + String(flags) + "\n";
        str += "Error: " + String(error) + "\n";
        str += "Length: " + String(length) + "\n";
        str += "Payload: ";
        for (size_t i = 0; i < length; i++) {
            str += String((char)payload[i]);
        }
        str += "\n}";
        return str;
    }
};

// Define the NexusPeerPendingEvent structure
struct NexusPeerEvent {
    MacAddress addr; // MAC address of the peer
    uint16_t seqNum; // Sequence number of the event
    uint16_t flags; // Flags for the event
    uint8_t error; // Error code for the event
    uint32_t time; // Time of the event
};

// Declare the Nexus namespace
namespace Nexus {
    const size_t MAX_PEERS = 20; // Maximum number of peers

    int CHANNEL = 0; // Channel for communication

    // Address variables for Nexus
    const uint8_t BROADCAST_ADDRESS_ARRAY[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Broadcast address array
    const MacAddress BROADCAST_ADDRESS(BROADCAST_ADDRESS_ARRAY); // Broadcast address
    MacAddress THIS_ADDRESS; // MAC address of this device

    bool (* onPeerConnect)(const MacAddress &peer); // Callback function for peer connection
    void (* onPeerDisconnect)(const MacAddress &peer, uint8_t error); // Callback function for peer disconnection
    void (* onScanComplete)(); // Callback function for scan completion
    bool (* onThisScanned)(const MacAddress &who); // Callback function for when this device is scanned

    uint32_t lastScan = 0; // Last scan time

    HyperList<MacAddress> peers; // List of peers
    HyperList<MacAddress> scanResults; // List of scan results

    HyperList<MacAddress> whitelist; // List of whitelisted peers
    HyperList<MacAddress> blacklist; // List of blacklisted peers
    HyperMap<MacAddress,uint8_t> blacklistReasons; // List of blacklist reasons

    PacketBuffer<NexusPacket> packetBuffer(NEXUS_BUFFER_SIZE); // Packet buffer
    HyperList<NexusPeerEvent> pendingPeersEvents_incoming; // List of incoming pending peer events
    HyperList<NexusPeerEvent> pendingPeersEvents_outgoing; // List of outgoing pending peer events

    /**
     * @brief Add a peer to the list of peers.
     * 
     * @param peer The MAC address of the peer to add.
     * @return uint8_t Error code indicating the result of the operation.
     *         - NEXUS_ERROR_OK: Peer added successfully.
     *         - NEXUS_ERROR_PEER_ALREADY_EXISTS: Peer already exists.
     *         - NEXUS_ERROR_PEER_FULL: Peer list is full.
     *         - NEXUS_ERROR_PEER_REFUSED: Peer refused.
     */
    uint8_t addPeer(const MacAddress &peer);
    
    /**
     * @brief Remove a peer from the list of peers.
     * 
     * @param peer The MAC address of the peer to remove.
     * @return uint8_t Error code indicating the result of the operation.
     *         - NEXUS_ERROR_OK: Peer removed successfully.
     *         - NEXUS_ERROR_PEER_NOT_FOUND: Peer not found.
     */
    uint8_t removePeer(const MacAddress &peer);
    
    /**
     * @brief Add a peer to the blacklist.
     * 
     * @param peer The MAC address of the peer to blacklist.
     * @param reason The reason for blacklisting the peer.
     */
    void addToBlacklist(const MacAddress &peer, uint8_t reason);
    
    /**
     * @brief Generate a random sequence number.
     * 
     * @return uint16_t A random sequence number.
     */
    uint16_t randomSequenceNum();
    
    /**
     * @brief Initialize the Nexus module.
     * 
     * @param channel The channel to use for communication.
     * @return bool True if initialization was successful, false otherwise.
     */
    bool begin(int channel = 0);
    
    /**
     * @brief Deinitialize the Nexus module.
     */
    void end();
    
    /**
     * @brief Check if a peer exists.
     * 
     * @param peer The MAC address of the peer to check.
     * @return bool True if the peer exists, false otherwise.
     */
    bool isPeer(const MacAddress &peer);
    
    /**
     * @brief Send a NexusPacket to a specific destination.
     * 
     * @param packet The packet to send.
     * @param destination The MAC address of the destination.
     * @return bool True if the packet was sent successfully, false otherwise.
     */
    bool sendPacket(const NexusPacket &packet, const MacAddress destination);
    
    /**
     * @brief Read a NexusPacket.
     * 
     * @param packet The packet to read.
     * @return bool True if a packet was read successfully, false otherwise.
     */
    bool readPacket(NexusPacket &packet);
    
    /**
     * @brief Send data to a specific destination.
     * 
     * @param data The data to send.
     * @param length The length of the data.
     * @param destination The MAC address of the destination.
     * @return bool True if the data was sent successfully, false otherwise.
     */
    bool sendData(const uint8_t *data, size_t length, const MacAddress &destination = BROADCAST_ADDRESS);
    
    /**
     * @brief Synchronize with a specific peer.
     * 
     * @param destination The MAC address of the peer to synchronize with.
     */
    void syncronize(const MacAddress &destination);
    
    /**
     * @brief Finish the connection with a specific peer.
     * 
     * @param destination The MAC address of the peer to finish the connection with.
     */
    void finish(const MacAddress &destination);
    
    /**
     * @brief Reset the connection with a specific peer.
     * 
     * @param destination The MAC address of the peer to reset the connection with.
     */
    void reset(const MacAddress &destination);
    
    /**
     * @brief Scan for nearby devices.
     */
    void scan();
    
    /**
     * @brief Loop function for the Nexus module.
     */
    void loop();
}

/**
 * @brief Callback function for receiving packets.
 * 
 * @param mac The MAC address of the sender.
 * @param data The data received.
 * @param len The length of the data.
 */
void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
    MacAddress mac_addr(mac); // Convert MAC address
    NexusPacket packet(0, 0, 0, 0, nullptr); // Create a new packet
    memcpy(&packet, data, len); // Copy the data to the packet

    Serial.println("Received From - " + mac_addr.toString());
    Serial.println("Received - " + packet.toString());

    uint32_t now = millis(); // Get the current time
    uint8_t error = NEXUS_ERROR_OK; // Initialize error code

    // Check if the peer is in the blacklist
    if (Nexus::blacklist.contains(mac_addr)) return;

    // Check the version of the packet
    if (packet.version != NEXUS_VERSION) {
        Nexus::pendingPeersEvents_outgoing.addend({mac_addr, ++packet.sequenceNum, NEXUS_FLAG_NONE, NEXUS_ERROR_INVALID_VERSION, now});
        Nexus::addToBlacklist(mac_addr, NEXUS_ERROR_INVALID_VERSION); // Add to blacklist if version is invalid
        return;
    }

    int index = 0;
    switch (packet.flags) {
    case NEXUS_FLAG_NONE:
        if(packet.length > 0 && packet.length <= NEXUS_MAX_PAYLOAD_SIZE) {
            if (Nexus::packetBuffer.isFull()) {
                Nexus::pendingPeersEvents_outgoing.addend({mac_addr, ++packet.sequenceNum , NEXUS_FLAG_ACK, NEXUS_ERROR_BUFFER_FULL, now});
                break;
            } else {
                Nexus::packetBuffer.enqueue(packet); // Enqueue the packet
                Nexus::pendingPeersEvents_outgoing.addend({mac_addr, ++packet.sequenceNum ,NEXUS_FLAG_ACK, NEXUS_ERROR_OK, now});
            }
        } else {
            Nexus::pendingPeersEvents_outgoing.addend({mac_addr, ++packet.sequenceNum ,NEXUS_FLAG_ACK, NEXUS_ERROR_INVALID_LENGTH, now});
        }
        break;
    case NEXUS_FLAG_SYN:
        if (!Nexus::whitelist.contains(mac_addr) && Nexus::onPeerConnect != nullptr) {
            if (!Nexus::onPeerConnect(mac_addr)) {
                Nexus::pendingPeersEvents_outgoing.addend({mac_addr, ++packet.sequenceNum ,NEXUS_FLAG_SYN | NEXUS_FLAG_ACK, NEXUS_ERROR_PEER_REFUSED, now});
                break;
            }
        }
        Nexus::addPeer(mac_addr); // Add the peer
        Nexus::pendingPeersEvents_outgoing.addend({mac_addr, ++packet.sequenceNum ,NEXUS_FLAG_SYN | NEXUS_FLAG_ACK, NEXUS_ERROR_OK, now});
        break;
    case NEXUS_FLAG_FIN:
        Nexus::removePeer(mac_addr); // Remove the peer
        Nexus::pendingPeersEvents_outgoing.addend({mac_addr, ++packet.sequenceNum ,NEXUS_FLAG_FIN | NEXUS_FLAG_ACK, NEXUS_ERROR_OK, now});
        break;
    case NEXUS_FLAG_RST:
        Nexus::removePeer(mac_addr); // Remove the peer
        Nexus::addPeer(mac_addr); // Add the peer again
        Nexus::pendingPeersEvents_outgoing.addend({mac_addr, ++packet.sequenceNum ,NEXUS_FLAG_RST | NEXUS_FLAG_ACK, NEXUS_ERROR_OK, now});
        break;
    case NEXUS_FLAG_SCN:
        if (!Nexus::whitelist.contains(mac_addr) && Nexus::onThisScanned != nullptr) {
            if (!Nexus::onThisScanned(mac_addr)) {
                // Do not respond to scan
                break;
            }
        }
        Nexus::pendingPeersEvents_outgoing.addend({mac_addr, ++packet.sequenceNum ,NEXUS_FLAG_SCN | NEXUS_FLAG_ACK, NEXUS_ERROR_OK, now});
        break;
    default:
        Serial.println("Flag - " + String(packet.flags));
        for (size_t index = 0; index < Nexus::pendingPeersEvents_incoming.size(); index++) {
            auto pendingEvent = Nexus::pendingPeersEvents_incoming[index];
            if (pendingEvent.addr == mac_addr && pendingEvent.seqNum == packet.sequenceNum && pendingEvent.flags == packet.flags) {
                switch (pendingEvent.flags) {
                case NEXUS_FLAG_SYN | NEXUS_FLAG_ACK:
                    Nexus::addPeer(mac_addr); // Add the peer
                    break;
                case NEXUS_FLAG_FIN | NEXUS_FLAG_ACK:
                    Nexus::removePeer(mac_addr); // Remove the peer
                    break;
                case NEXUS_FLAG_RST | NEXUS_FLAG_ACK:
                    Nexus::removePeer(mac_addr); // Remove the peer
                    Nexus::addPeer(mac_addr); // Add the peer again
                    break;
                case NEXUS_FLAG_SCN | NEXUS_FLAG_ACK:
                    if (Nexus::scanResults.contains(mac_addr)) break;
                    Serial.println("Adding to scan results");
                    Nexus::scanResults.addend(mac_addr); // Add to scan results
                    break;
                default:
                    Nexus::pendingPeersEvents_outgoing.addend({mac_addr, ++packet.sequenceNum ,NEXUS_FLAG_NONE, NEXUS_ERROR_INVALID_FLAG, now});
                    Nexus::addToBlacklist(mac_addr, NEXUS_ERROR_INVALID_FLAG); // Add to blacklist if flag is invalid
                    break;
                }
                Nexus::pendingPeersEvents_incoming.remove(index); // Remove the pending event
            }
            index++;
        }
        break;
    }
}


/**
 * @brief Callback function for sending packets.
 * 
 * @param mac_addr The MAC address of the recipient.
 * @param status The status of the send operation.
 */
void onSend(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status == ESP_NOW_SEND_SUCCESS) {
        // Packet sent successfully
    } else {
        Nexus::reset(MacAddress(mac_addr)); // Reset the peer
    }
}

// Define the Nexus namespace
namespace Nexus {
    // Add a peer to the list of peers
    uint8_t addPeer(const MacAddress &peer) {
        if (peers.contains(peer)) return NEXUS_ERROR_PEER_ALREADY_EXISTS; // Check if peer already exists
        esp_now_peer_info_t peerInfo = {}; // Create a peer info structure
        peer.toBuffer(peerInfo.peer_addr); // Copy the peer address
        peerInfo.channel = CHANNEL; // Set the channel
        peerInfo.encrypt = false; // Disable encryption
        switch(esp_now_add_peer(&peerInfo)){
            case ESP_OK:
                if (peer != BROADCAST_ADDRESS) peers.addend(peer); // Add the peer if not broadcast
                return NEXUS_ERROR_OK; // Peer added successfully
            case ESP_ERR_ESPNOW_FULL:
                return NEXUS_ERROR_PEER_FULL; // Peer list is full
            case ESP_ERR_ESPNOW_EXIST:
                return NEXUS_ERROR_PEER_ALREADY_EXISTS; // Peer already exists
             default:
                return NEXUS_ERROR_PEER_REFUSED; // Peer refused
        }
    }

    // Remove a peer from the list of peers
    uint8_t removePeer(const MacAddress &peer) {
        peers.removeValue(peer); // Remove the peer
        switch(esp_now_del_peer(peer.addr)){
            case ESP_OK:
                return NEXUS_ERROR_OK; // Peer removed successfully
            case ESP_ERR_ESPNOW_NOT_FOUND:
                return NEXUS_ERROR_PEER_NOT_FOUND; // Peer not found
            default:
                return NEXUS_ERROR_PEER_NOT_FOUND; // Peer not found
        }
    }

    // Add a peer to the blacklist
    void addToBlacklist(const MacAddress &peer, uint8_t reason) {
        if (blacklist.contains(peer)) return; // Check if peer is already blacklisted
        blacklist.addend(peer); // Add the peer to the blacklist
        blacklistReasons.put(peer, reason); // Add the reason to the blacklist
    }

    // Generate a random sequence number
    uint16_t randomSequenceNum() {
        return random(0, 65535); // Return a random sequence number
    }

    // Initialize the Nexus module
    bool begin(int channel) {
        CHANNEL = channel; // Set the channel

        WiFi.mode(WIFI_STA); // Set the WiFi mode to station
        WiFi.begin(); // Begin WiFi

        // Get the MAC address of the ESP32
        uint8_t mac[6]; // Create a buffer for the MAC address
        //if(esp_base_mac_addr_get(mac) != ESP_OK) return false;
        if (esp_wifi_get_mac(WIFI_IF_STA, mac) != ESP_OK) return false;
        THIS_ADDRESS = MacAddress(mac); // Set the MAC address of this device

        WiFi.disconnect(true,true); // Disconnect from WiFi and turn off the WiFi radio

        // Initialize ESP-NOW
        WiFi.mode(WIFI_STA); // Set the WiFi mode to station
        if (esp_now_init() != ESP_OK) return false; // Initialize ESP-NOW



        // Register the callbacks
        if (esp_now_register_recv_cb(onReceive) != ESP_OK) return false;
        if (esp_now_register_send_cb(onSend) != ESP_OK) return false;

        if (addPeer(BROADCAST_ADDRESS) != NEXUS_ERROR_OK) return false; // Add the broadcast peer

        return true; // Initialization successful
    }

    // Deinitialize the Nexus module
    void end() {
        esp_now_deinit(); // Deinitialize ESP-NOW
    }

    // Check if a peer exists
    bool isPeer(const MacAddress &peer) {
        return peers.contains(peer); // Check if the peer exists
    }

    // Send a NexusPacket to a specific destination
    bool sendPacket(const NexusPacket &packet, const MacAddress destination) {
        //Serial.println("Sending To - " + destination.toString());
        //Serial.println("Sending - " + packet.toString());
        uint8_t data[packet.size()]; // Create a buffer for the packet
        memcpy(data, &packet, packet.size()); // Copy the packet to the buffer
        return esp_now_send(destination.addr, data, packet.size()) == ESP_OK; // Send the packet
    }

    // Read a NexusPacket
    bool readPacket(NexusPacket &packet) {
        return packetBuffer.dequeue(packet); // Dequeue the packet
    }

    // Send data to a specific destination
    bool sendData(const uint8_t *data, size_t length, const MacAddress destination) {
        if (length > NEXUS_MAX_PAYLOAD_SIZE) return false; // Check if length is valid
        NexusPacket packet(randomSequenceNum(), NEXUS_FLAG_NONE, NEXUS_ERROR_OK, length, data); // Create a new packet
        pendingPeersEvents_incoming.addend({destination, ++packet.sequenceNum, NEXUS_FLAG_ACK, NEXUS_ERROR_OK, millis()});
        return sendPacket(packet, destination); // Send the packet
    }

    // Synchronize with a specific peer
    void syncronize(const MacAddress &destination) {
        NexusPacket packet(randomSequenceNum(), NEXUS_FLAG_SYN, NEXUS_ERROR_OK, 0, nullptr); // Create a new packet
        pendingPeersEvents_incoming.addend({destination, ++packet.sequenceNum, NEXUS_FLAG_SYN | NEXUS_FLAG_ACK, NEXUS_ERROR_OK, millis()});
        sendPacket(packet, destination); // Send the packet
    }

    // Finish the connection with a specific peer
    void finish(const MacAddress &destination) {
        NexusPacket packet(randomSequenceNum(), NEXUS_FLAG_FIN, NEXUS_ERROR_OK, 0, nullptr); // Create a new packet
        pendingPeersEvents_incoming.addend({destination, ++packet.sequenceNum, NEXUS_FLAG_FIN | NEXUS_FLAG_ACK, NEXUS_ERROR_OK, millis()});
        sendPacket(packet, destination); // Send the packet
    }

    // Reset the connection with a specific peer
    void reset(const MacAddress &destination) {
        if (isPeer(destination))
        {
            NexusPacket packet(randomSequenceNum(), NEXUS_FLAG_RST, NEXUS_ERROR_OK, 0, nullptr); // Create a new packet
            pendingPeersEvents_incoming.addend({destination, ++packet.sequenceNum, NEXUS_FLAG_RST | NEXUS_FLAG_ACK, NEXUS_ERROR_OK, millis()});
            sendPacket(packet, destination); // Send the packet
        }
    }

    // Scan for nearby devices
    void scan() {
        NexusPacket packet(randomSequenceNum(), NEXUS_FLAG_SCN, NEXUS_ERROR_OK, 0, nullptr); // Create a new packet
        scanResults.clear(); // Clear the scan results
        for (size_t i = 0; i < peers.size(); i++) {
            pendingPeersEvents_incoming.addend({peers[i], ++packet.sequenceNum, NEXUS_FLAG_SCN | NEXUS_FLAG_ACK, NEXUS_ERROR_OK, millis()});
        }
        packet.sequenceNum--;
        sendPacket(packet, BROADCAST_ADDRESS); // Send the packet
    }

    // Loop function for the Nexus module
    void loop() {
        uint32_t now = millis(); // Get the current time
        // Check for timeouts in pending incoming events
        for (size_t i = 0; i < pendingPeersEvents_incoming.size(); i++) {
            auto pendingEvent = pendingPeersEvents_incoming[i];
            if (now - pendingEvent.time > NEXUS_TIMEOUT) {
                pendingPeersEvents_outgoing.addend({pendingEvent.addr, pendingEvent.seqNum, NEXUS_FLAG_NONE, NEXUS_ERROR_TIMEOUT, now});
                reset(pendingEvent.addr); // Reset the peer if timeout
                pendingPeersEvents_incoming.remove(i); // Remove the pending event
            }
        }

        // Send pending outgoing events
        while (pendingPeersEvents_outgoing.size() > 0) {
            auto pendingEvent = pendingPeersEvents_outgoing[0];
            sendPacket(NexusPacket(pendingEvent.seqNum, pendingEvent.flags, pendingEvent.error, 0, nullptr), pendingEvent.addr); // Send the packet
            Serial.println("Sending Flag - " + pendingEvent.flags);
            if (pendingEvent.flags & (NEXUS_FLAG_FIN | NEXUS_FLAG_RST) && onPeerDisconnect != nullptr) {
                onPeerDisconnect(pendingEvent.addr, pendingEvent.error); // Call the disconnect callback
            }
            pendingPeersEvents_outgoing.remove(0); // Remove the pending event
        }

        // Check for scan interval
        if (now - lastScan > NEXUS_SCAN_INTERVAL) {
            if (onScanComplete != nullptr) {
                onScanComplete(); // Call the scan complete callback
            }
            scan(); // Perform a scan
            lastScan = now; // Update the last scan time
        }
    }
}

#endif // NEXUS_HPP