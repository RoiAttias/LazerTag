#ifndef NEXUS_HPP
#define NEXUS_HPP

#include <Arduino.h> // Include the Arduino library
#include <WiFi.h> // Include the WiFi library
#include <esp_now.h> // Include the ESP-NOW library

#include "Utilities/PacketBuffer.hpp" // Include the PacketBuffer utility
#include "Utilities/HyperList.hpp" // Include the HyperList utility

// Define constants for Nexus
#define NEXUS_TIMEOUT 1000 // Timeout for Nexus operations
#define NEXUS_SCAN_INTERVAL 5000 // Interval for scanning devices
#define NEXUS_BUFFER_SIZE 32 // Buffer size for packets
#define NEXUS_HEADER_SIZE 12 // Header size for packets
#define NEXUS_MAX_PAYLOAD_SIZE (ESP_NOW_MAX_DATA_LEN - NEXUS_HEADER_SIZE) // Maximum payload size

#define NEXUS_VERSION 0x01 // Version of Nexus

struct _attribute__((packed)) NexusAddress {
    uint8_t projectID;
    uint8_t groups;
    uint8_t deviceID;

    NexusAddress(uint8_t projectID = 0, uint8_t groups = 0, uint8_t deviceID = 0)
        : projectID(projectID), groups(groups), deviceID(deviceID) {}

    void toBuffer(uint8_t buffer[]) {
        buffer[0] = projectID;
        buffer[1] = groups;
        buffer[2] = deviceID;
    }

    String toString() const {
        return String(projectID) + "." + String(groups) + "." + String(deviceID);
    }
};

// Define the NexusPacket structure
struct __attribute__((packed)) NexusPacket {
    uint8_t version; // 1 byte
    NexusAddress source; // 3 bytes
    NexusAddress destination; // 3 bytes
    uint16_t sequenceNum; // 2 bytes
    uint16_t command; // 2 bytes
    uint8_t length; // 1 byte
    // Header size: 12 bytes
    uint8_t payload[NEXUS_MAX_PAYLOAD_SIZE]; // 238 bytes

    // Constructor for NexusPacket
    NexusPacket(NexusAddress source, NexusAddress destination, uint16_t sequenceNum, uint16_t command, uint8_t length, const uint8_t payload[])
        : version(NEXUS_VERSION), source(source), destination(destination), sequenceNum(sequenceNum), uint16_t command, length(length) {
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
        str += "Source: " + source.toString() + "\n";
        str += "Destination: " + destination.toString() + "\n";
        str += "SequenceNum: " + String(sequenceNum) + "\n";
        str += "Command: " + String(command) + "\n";
        str += "Length: " + String(length) + "\n";
        str += "Payload: ";
        for (size_t i = 0; i < length; i++) {
            str += String((char)payload[i]);
        }
        str += "\n}";
        return str;
    }
};

// Declare the Nexus namespace
namespace Nexus {
    // ESP-NOW Variables
    int CHANNEL = 0; // Channel for communication
    const uint8_t BROADCAST_MAC_ADDRESS[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Broadcast address array

    // Callback functions
    void (* onDeviceConnected)(const NexusAddress &who); // Callback function for when a device is connected
    void (* onDeviceDisconnected)(const NexusAddress &who); // Callback function for when a device is disconnected
    void (* onScanComplete)(); // Callback function for scan completion
    bool (* onThisScanned)(const NexusAddress &who); // Callback function for when this device is scanned
    void (* onPacketReceived)(const NexusPacket &packet); // Callback function for when a packet is received

    // Scan variables
    uint32_t lastScan = 0; // Last scan time
    uint16_t scanSeq = 0; // Sequence number
    bool isScanComplete = false; // Scan completion flag
    bool shouldScan = true; // Scan flag

    NexusAddress THIS_ADDRESS(0, 0, 0); // Address of this device

    HyperList<NexusAddress> devices; // List of devices
    HyperList<NexusAddress> scanResults; // List of scan results

    PacketBuffer<NexusPacket> incomingBuffer(NEXUS_BUFFER_SIZE);
    HyperList<NexusPacket> outgoingPackets(NEXUS_BUFFER_SIZE);
    
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
    bool begin();
    
    /**
     * @brief Deinitialize the Nexus module.
     */
    void end();
    
    /**
     * @brief Send a NexusPacket to a specific destination.
     * 
     * @param packet The packet to send.
     * @param destination The address of the destination.
     * @return bool True if the packet was sent successfully, false otherwise.
     */
    bool sendPacket(const NexusPacket &packet, const NexusAddress destination);
    
    /**
     * @brief Send data to a specific destination.
     * 
     * @param command The command to send.
     * @param length The length of the data.
     * @param data The data to send.
     * @param destination The address of the destination.
     * @return bool True if the data was sent successfully, false otherwise.
     */
    bool sendData(uint16_t command, uint8_t length, const uint8_t data[], const NexusAddress destination);

    /**
     * @brief Read a NexusPacket.
     * 
     * @param packet Reference to the packet to store the data.
     * @return bool True if a packet was read successfully, false otherwise.
     */
    bool readPacket(NexusPacket &packet);
    
    /**
     * @brief Check if a packet is available.
     * 
     * @return int The number of packets available.
     */
    int available();

    /**
     * @brief Scan for nearby devices.
     */
    void scan();

    /**
     * @brief Set the address of this device.
     * 
     * @param projectID The project ID.
     * @param groups The groups to join.
     * @param deviceID The device ID.
     */
    void setAddress(uint8_t projectID, uint8_t groups, uint8_t deviceID) {
        THIS_ADDRESS.projectID = projectID;
        THIS_ADDRESS.groups = groups;
        THIS_ADDRESS.deviceID = deviceID;
    }

    /**
     * @brief Join a group.
     * 
     * @param groupID The ID of the group to join (0-7).
     */
    void joinGroup(uint8_t groupID) {
        THIS_ADDRESS.groups |= (1 << groupID);
    }

    /**
     * @brief Leave a group.
     * 
     * @param groupID The ID of the group to leave (0-7).
     */
    
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
    NexusPacket packet;
    memcpy(&packet, data, len); // Copy the data to the packet

     // Check the version
    if (packet.version != NEXUS_VERSION) return;
    
    // Check if the packet is for this device
    if ((packet.destination.projectID == Nexus::THIS_ADDRESS.projectID) &&
        (packet.destination.groups | Nexus::THIS_ADDRESS.groups) &&
        (packet.destination.deviceID == Nexus::THIS_ADDRESS.deviceID || packet.destination.deviceID == 255)) {
        if (packet.command == 0 && packet.length == 0) {
            if (Nexus::scanSeq == packet.sequenceNum) {
                Nexus::scanResults.add(packet.source);
            } else {
                if (Nexus::onThisScanned != nullptr) {
                    if(!Nexus::onThisScanned(packet.source)) {
                        return;
                    }
                }
            Nexus::outgoingPackets.add(NexusPacket(Nexus::THIS_ADDRESS, packet.source, ++packet.sequenceNum, 0, 0, nullptr));
        }
        else {
            Nexus::incomingBuffer.enqueue(packet); // Enqueue the packet
            if (Nexus::onPacketReceived != nullptr) {
                Nexus::onPacketReceived(packet); // Call the packet received callback
            }
        }
    }
}

// Define the Nexus namespace
namespace Nexus {
    // Generate a random sequence number
    uint16_t randomSequenceNum() {
        return random(0, 65535); // Return a random sequence number
    }

    // Initialize the Nexus module
    bool begin() {
        // Initialize ESP-NOW
        WiFi.mode(WIFI_STA); // Set the WiFi mode to station
        if (esp_now_init() != ESP_OK) return false; // Initialize ESP-NOW

        // Register the callbacks
        if (esp_now_register_recv_cb(onReceive) != ESP_OK) return false;

        // Add the broadcast peer
        esp_now_peer_info_t peerInfo = {}; // Create a peer info structure
        memcpy(peerInfo.peer_addr, BROADCAST_MAC_ADDRESS, 6); // Copy the broadcast address
        peerInfo.channel = CHANNEL; // Set the channel
        peerInfo.encrypt = false; // Disable encryption
        if(esp_now_add_peer(&peerInfo) != ESP_OK) return false; // Add the broadcast peer

        return true; // Initialization successful
    }

    // Deinitialize the Nexus module
    void end() {
        esp_now_deinit(); // Deinitialize ESP-NOW
    }

    // Send a NexusPacket to a specific destination
    bool sendPacket(const NexusPacket &packet) {
        if (packet.size() > ESP_NOW_MAX_DATA_LEN) return false; // Check if packet size is valid
        uint8_t data[packet.size()]; // Create a data buffer
        memcpy(data, &packet, packet.size()); // Copy the packet to the data buffer
        esp_err_t result = esp_now_send(BROADCAST_MAC_ADDRESS, data, packet.size()); // Send the data
        return result == ESP_OK; // Return the result
    }

    // Read a NexusPacket
    bool readPacket(NexusPacket &packet) {
        return incomingBuffer.dequeue(packet); // Dequeue the packet
    }

    // Return the number of bytes available
    int available() {
        return packetBuffer.size();
    }

    // Send data to a specific destination
    bool sendData(const NexusAddress &destination, uint16_t command, uint8_t length, const uint8_t data[]) {
        NexusPacket packet(THIS_ADDRESS, destination, randomSequenceNum(), command, length, data); // Create a new packet
        return sendPacket(packet, destination); // Send the packet
    }

    // Scan for nearby devices
    void scan() {
        sendData(0, 0, nullptr, NexusAddress(255,255,255)); // Send a scan packet
        scanResults.clear(); // Clear the scan results
        scanSeq = packet.sequenceNum; // Update the scan sequence number
        sendPacket(packet, BROADCAST_ADDRESS); // Send the packet
    }

    // Loop function for the Nexus module
    void loop() {
        uint32_t now = millis(); // Get the current time

        // Send outgoing packets
        while (outgoingPackets.size() > 0) {
            auto packet = outgoingPackets[0]; // get the next packet
            sendPacket(packet); // Send the packet
            outgoingPackets.remove(0); // Remove the current packet
        }

        // Check for scan interval
        if (now - lastScan > NEXUS_SCAN_INTERVAL) {
            isScanComplete = true; // Set the scan completion flag

            // Check for new devices
            for (size_t i = 0; i < scanResults.size(); i++) {
                auto device = scanResults[i]; // Get the device
                if (!devices.contains(device)) {
                    devices.add(device); // Add the device to the list
                    if (onDeviceConnected != nullptr) {
                        onDeviceConnected(device); // Call the device connected callback
                    }
                }
            }

            // Check for disconnected devices
            for (size_t i = 0; i < devices.size(); i++) {
                auto device = devices[i]; // Get the device
                if (!scanResults.contains(device)) {
                    devices.remove(i); // Remove the device from the list
                    if (onDeviceDisconnected != nullptr) {
                        onDeviceDisconnected(device); // Call the device disconnected callback
                    }
                }
            }

            if (onScanComplete != nullptr) {
                onScanComplete(); // Call the scan complete callback
            }
            if (shouldScan)
            {
                scan(); // Perform a scan
                lastScan = now; // Update the last scan time
            }
        }
    }
}

#endif // NEXUS_HPP