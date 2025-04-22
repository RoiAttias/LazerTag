/**
 * @file Nexus.hpp
 * @brief Declarations for the Nexus ESP-NOW networking module on ESP32.
 *
 * Provides packed structures for device addressing and packet framing,
 * constants for protocol configuration, and the public API in the Nexus namespace.
 */

 #ifndef NEXUS_HPP
 #define NEXUS_HPP
 
 #include <Arduino.h>
 #include <WiFi.h>
 #include <esp_now.h>
 #include "Utilities/PacketBuffer.hpp"
 #include "Utilities/HyperList.hpp"
 
 // ---------------------- CONSTANTS ----------------------
 /** Interval (ms) between automatic network scans. */
 #define NEXUS_SCAN_INTERVAL 2000
 /** Capacity of the incoming packet buffer. */
 #define NEXUS_BUFFER_SIZE 64
 /** Header length (bytes) for a NexusPacket. */
 #define NEXUS_HEADER_SIZE 12
 /** Maximum payload size (bytes) for a NexusPacket. */
 #define NEXUS_MAX_PAYLOAD_SIZE (ESP_NOW_MAX_DATA_LEN - NEXUS_HEADER_SIZE)
 
 /** Special sequence number indicating a scan command. */
 static const uint16_t NEXUS_COMMAND_SCAN = static_cast<uint16_t>(-1);
 /** Protocol version identifier. */
 #define NEXUS_VERSION 0x01
 
 // ------------------- ADDRESS STRUCTURE -------------------
 /**
  * @brief Packed device address (project ID, group mask, device ID).
  *
  * Used to identify and group devices in the Nexus network.
  */
 struct __attribute__((packed)) NexusAddress {
     uint8_t projectID;   ///< Project or application identifier
     uint8_t groups;      ///< Bitmask of group memberships
     uint8_t deviceID;    ///< Unique device identifier within the project/groups
 
     /**
      * @brief Constructs a NexusAddress.
      *
      * @param projectID Project identifier (default 0).
      * @param groups    Group mask (default 0).
      * @param deviceID  Device identifier (default 0).
      */
     NexusAddress(uint8_t projectID = 0, uint8_t groups = 0, uint8_t deviceID = 0);
 
     /**
      * @brief Serialize the address into a 3-byte buffer.
      *
      * @param buffer Pointer to at least 3-byte array.
      */
     void toBuffer(uint8_t buffer[]) const;
 
     /**
      * @brief Convert the address to a human-readable string.
      *
      * Format: "project.groups.device".
      * @return String representation of the address.
      */
     String toString() const;
 
     /** Equality comparison operator. */
     bool operator==(const NexusAddress &other) const;
     /** Inequality comparison operator. */
     bool operator!=(const NexusAddress &other) const;
 };
 
 // -------------------- PACKET STRUCTURE --------------------
 /**
  * @brief Packed network packet frame for the Nexus protocol.
  *
  * Contains header fields and a payload up to NEXUS_MAX_PAYLOAD_SIZE bytes.
  */
 struct __attribute__((packed)) NexusPacket {
     uint8_t version;           ///< Protocol version (NEXUS_VERSION)
     NexusAddress source;       ///< Sender address
     NexusAddress destination;  ///< Recipient address
     uint16_t sequenceNum;      ///< Packet sequence number
     uint16_t command;          ///< Command code or scan indicator
     uint8_t length;            ///< Payload length in bytes
     uint8_t payload[NEXUS_MAX_PAYLOAD_SIZE]; ///< Raw payload data
 
     /**
      * @brief Construct a NexusPacket with optional payload.
      *
      * @param source      Sender address (default 0.0.0)
      * @param destination Recipient address (default 0.0.0)
      * @param sequenceNum Packet sequence number (default 0)
      * @param command     Command or scan code (default 0)
      * @param length      Payload byte count (default 0)
      * @param payload     Pointer to payload data (nullable)
      */
     NexusPacket(
         NexusAddress source = NexusAddress(),
         NexusAddress destination = NexusAddress(),
         uint16_t sequenceNum = 0,
         uint16_t command = 0,
         uint8_t length = 0,
         const uint8_t payload[] = nullptr);
 
     /**
      * @brief Calculate total packet size (header + payload).
      * @return Number of bytes to transmit over ESP-NOW.
      */
     size_t size() const;
 
     /**
      * @brief Convert packet fields and payload to a debug string.
      * @return Multi-line string summarizing packet contents.
      */
     String toString() const;
 };
 
 /**
  * @brief ESP-NOW receive callback signature.
  *
  * Called by ESP-NOW on packet reception.
  * @param mac 6-byte MAC address of sender.
  * @param data Raw received data pointer.
  * @param len  Length of data in bytes.
  */
 void onReceive(const uint8_t *mac, const uint8_t *data, int len);
 
 // ------------------- NEXUS NAMESPACE API -------------------
 /**
  * @namespace Nexus
  * @brief Core API and state for Nexus networking.
  */
 namespace Nexus {
 
     extern int CHANNEL;                           ///< WiFi channel for ESP-NOW
     extern const uint8_t BROADCAST_MAC_ADDRESS[6];///< Broadcast MAC address
 
     extern void (* onDeviceConnected)(const NexusAddress &who);   ///< Called when a new device is discovered
     extern void (* onDeviceDisconnected)(const NexusAddress &who);///< Called when a device is removed
     extern void (* onScanComplete)();                             ///< Called when scanning completes
     extern bool (* onThisScanned)(const NexusAddress &who);       ///< Predicate when receiving scan request
     extern void (* onPacketReceived)(const NexusPacket &packet);  ///< Called on inbound data packet
 
     extern uint32_t lastScan;      ///< Timestamp of last scan execution
     extern uint16_t scanSeq;       ///< Sequence number used for scan packets
     extern bool isScanComplete;    ///< Flag indicating scan end
     extern bool shouldScan;        ///< Flag to trigger next scan
 
     extern NexusAddress THIS_ADDRESS;               ///< Local device address
     extern HyperList<NexusAddress> devices;        ///< List of known peers
     extern HyperList<NexusAddress> scanResults;    ///< Peers found in current scan
     extern PacketBuffer<NexusPacket> incomingBuffer;///< Buffer for inbound packets
     extern HyperList<NexusPacket> outgoingPackets; ///< Queue of outbound packets
 
     /** Set the local device address. */
     void setAddress(uint8_t projectID, uint8_t groups, uint8_t deviceID);
     /** Retrieve the current project ID. */
     uint8_t getProjectID();
     /** Compute group mask bit for a given group ID. */
     uint8_t calcGroupMask(uint8_t groupID);
     /** Join a group by setting its bit in the groups mask. */
     void joinGroup(uint8_t groupID);
     /** Leave a group by clearing its bit. */
     void leaveGroup(uint8_t groupID);
     /** Generate a random sequence number for packets. */
     uint16_t randomSequenceNum();
     /**
      * @brief Initialize ESP-NOW and Nexus networking.
      *
      * @param address Local NexusAddress to use.
      * @return True on success, false otherwise.
      */
     bool begin(const NexusAddress &address);
     /** Deinitialize ESP-NOW and clear state. */
     void end();
     /**
      * @brief Send a prepared NexusPacket to its destination.
      * @return True if send succeeded, false otherwise.
      */
     bool sendPacket(const NexusPacket &packet);
     /**
      * @brief Helper to build and send a packet with raw data.
      */
     bool sendData(uint16_t command, uint8_t length, uint8_t data[], const NexusAddress &destination);
     /** Send a command to a specific device ID. */
     bool sendToDevice(uint16_t command, uint8_t length, uint8_t data[], uint8_t deviceID);
     /** Send a command to all devices in a group. */
     bool sendToGroup(uint16_t command, uint8_t length, uint8_t data[], uint8_t groupID);
     /** Dequeue the next received packet. */
     bool readPacket(NexusPacket &packet);
     /** Get the count of packets waiting in the buffer. */
     int available();
     /** Trigger a network scan on next loop. */
     void scan();
     /** Handle Nexus tasks: send queue, scan, and callbacks. */
     void loop();
 }
 
 #endif // NEXUS_HPP 