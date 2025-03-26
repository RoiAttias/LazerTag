#ifndef NEXUS_HPP
#define NEXUS_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "Utilities/PacketBuffer.hpp"
#include "Utilities/HyperList.hpp"

// Define constants for Nexus
#define NEXUS_SCAN_INTERVAL 2000
#define NEXUS_BUFFER_SIZE 32
#define NEXUS_HEADER_SIZE 12
#define NEXUS_MAX_PAYLOAD_SIZE (ESP_NOW_MAX_DATA_LEN - NEXUS_HEADER_SIZE)
const uint16_t NEXUS_COMMAND_SCAN = -1;
#define NEXUS_VERSION 0x01

// NexusAddress structure declaration
struct __attribute__((packed)) NexusAddress {
    uint8_t projectID;
    uint8_t groups;
    uint8_t deviceID;

    NexusAddress(uint8_t projectID = 0, uint8_t groups = 0, uint8_t deviceID = 0);
    void toBuffer(uint8_t buffer[]);
    String toString() const;
    bool operator==(const NexusAddress &other) const;
    bool operator!=(const NexusAddress &other) const;
};

// NexusPacket structure declaration
struct __attribute__((packed)) NexusPacket {
    uint8_t version;
    NexusAddress source;
    NexusAddress destination;
    uint16_t sequenceNum;
    uint16_t command;
    uint8_t length;
    uint8_t payload[NEXUS_MAX_PAYLOAD_SIZE];

    NexusPacket(NexusAddress source = NexusAddress(), NexusAddress destination = NexusAddress(),
                uint16_t sequenceNum = 0, uint16_t command = 0, uint8_t length = 0, const uint8_t payload[] = nullptr);
    size_t size() const;
    String toString() const;
};

// Declaration of onReceive (to be defined in Nexus.cpp)
void onReceive(const uint8_t *mac, const uint8_t *data, int len);

namespace Nexus {
    // Global variables declarations
    extern int CHANNEL;
    extern const uint8_t BROADCAST_MAC_ADDRESS[6];

    extern void (* onDeviceConnected)(const NexusAddress &who);
    extern void (* onDeviceDisconnected)(const NexusAddress &who);
    extern void (* onScanComplete)();
    extern bool (* onThisScanned)(const NexusAddress &who);
    extern void (* onPacketReceived)(const NexusPacket &packet);

    extern uint32_t lastScan;
    extern uint16_t scanSeq;
    extern bool isScanComplete;
    extern bool readScan;
    extern bool shouldScan;

    extern NexusAddress THIS_ADDRESS;
    extern HyperList<NexusAddress> devices;
    extern HyperList<NexusAddress> scanResults;
    extern PacketBuffer<NexusPacket> incomingBuffer;
    extern HyperList<NexusPacket> outgoingPackets;

    // Function declarations
    void setAddress(uint8_t projectID, uint8_t groups, uint8_t deviceID);
    uint8_t getProjectID();
    uint8_t calcGroupMask(uint8_t groupID);
    void joinGroup(uint8_t groupID);
    void leaveGroup(uint8_t groupID);
    uint16_t randomSequenceNum();
    bool begin(const NexusAddress &address);
    void end();
    bool sendPacket(const NexusPacket &packet);
    bool sendData(uint16_t command, uint8_t length, uint8_t data[], const NexusAddress &destination);
    bool sendToDevice(uint16_t command, uint8_t length, uint8_t data[], const uint8_t deviceID);
    bool sendToGroup(uint16_t command, uint8_t length, uint8_t data[], const uint8_t groupID);
    bool readPacket(NexusPacket &packet);
    int available();
    void scan();
    void loop();
}

#endif // NEXUS_HPP