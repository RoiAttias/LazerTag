/**
 * @file Nexus.cpp
 * @brief Implementation of Nexus ESP-NOW networking: address, packet, and namespace functions.
 *
 * Defines constructors, serialization, utility functions, and the main loop
 * for peer discovery and reliable packet send/receive using PacketBuffer and HyperList.
 */

 #include "Nexus.hpp"
 #include <string.h>
 
 // --------------------- NexusAddress ---------------------
 /**
  * @brief Construct a NexusAddress with given IDs.
  */
 NexusAddress::NexusAddress(uint8_t projectID, uint8_t groups, uint8_t deviceID)
     : projectID(projectID), groups(groups), deviceID(deviceID) {}
 
 /**
  * @brief Serialize address into a 3-byte buffer.
  */
 void NexusAddress::toBuffer(uint8_t buffer[]) const {
     buffer[0] = projectID;
     buffer[1] = groups;
     buffer[2] = deviceID;
 }
 
 /**
  * @brief Convert address to string "proj.groups.device".
  */
 String NexusAddress::toString() const {
     return String(projectID) + "." + String(groups) + "." + String(deviceID);
 }
 
 /**
  * @brief Compare two NexusAddress for equality.
  */
 bool NexusAddress::operator==(const NexusAddress &other) const {
     return projectID == other.projectID && groups == other.groups && deviceID == other.deviceID;
 }
 
 /**
  * @brief Compare two NexusAddress for inequality.
  */
 bool NexusAddress::operator!=(const NexusAddress &other) const {
     return !(*this == other);
 }
 
 // --------------------- NexusPacket ---------------------
 /**
  * @brief Construct a packet with optional payload data.
  */
 NexusPacket::NexusPacket(NexusAddress source, NexusAddress destination,
                           uint16_t sequenceNum, uint16_t command, uint8_t length, const uint8_t payload[])
     : version(NEXUS_VERSION), source(source), destination(destination),
       sequenceNum(sequenceNum), command(command), length(length) {
     if (payload != nullptr) {
         memcpy(this->payload, payload, length);
     }
 }
 
 /**
  * @brief Calculate the total size of the packet (header + payload).
  */
 size_t NexusPacket::size() const {
     return NEXUS_HEADER_SIZE + length;
 }
 
 /**
  * @brief Format packet fields and payload into a human-readable string.
  */
 String NexusPacket::toString() const {
     String str = "NexusPacket {\n";
     str += "  Version: " + String(version) + "\n";
     str += "  Source: " + source.toString() + "\n";
     str += "  Dest: " + destination.toString() + "\n";
     str += "  Seq#: " + String(sequenceNum) + "\n";
     str += "  Cmd: " + String(command) + "\n";
     str += "  Len: " + String(length) + "\n";
     str += "  Payload: ";
     for (size_t i = 0; i < length; ++i) {
         str += String(payload[i], HEX) + " ";
     }
     str += "\n}";
     return str;
 }
 
 // ------------------- Nexus NAMESPACE ---------------------
 namespace Nexus {
     int CHANNEL = 0;
     const uint8_t BROADCAST_MAC_ADDRESS[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
 
     void (* onDeviceConnected)(const NexusAddress&)    = nullptr;
     void (* onDeviceDisconnected)(const NexusAddress&) = nullptr;
     void (* onScanComplete)()                         = nullptr;
     bool (* onThisScanned)(const NexusAddress&)       = nullptr;
     void (* onPacketReceived)(const NexusPacket&)     = nullptr;
 
     uint32_t lastScan        = 0;
     uint16_t scanSeq         = 0;
     bool isScanComplete      = false;
     bool shouldScan          = false;
 
     NexusAddress THIS_ADDRESS;
     HyperList<NexusAddress> devices;
     HyperList<NexusAddress> scanResults;
     PacketBuffer<NexusPacket> incomingBuffer(NEXUS_BUFFER_SIZE);
     HyperList<NexusPacket> outgoingPackets;
 
     void setAddress(uint8_t projectID, uint8_t groups, uint8_t deviceID) {
         THIS_ADDRESS = NexusAddress(projectID, groups, deviceID);
     }
 
     uint8_t getProjectID() {
         return THIS_ADDRESS.projectID;
     }
 
     uint8_t calcGroupMask(uint8_t groupID) {
         return 1 << groupID;
     }
 
     void joinGroup(uint8_t groupID) {
         THIS_ADDRESS.groups |= calcGroupMask(groupID);
     }
 
     void leaveGroup(uint8_t groupID) {
         THIS_ADDRESS.groups &= ~calcGroupMask(groupID);
     }
 
     uint16_t randomSequenceNum() {
         return static_cast<uint16_t>(random(0, 65535));
     }
 
     bool begin(const NexusAddress &address) {
         WiFi.mode(WIFI_STA);
         if (esp_now_init() != ESP_OK) return false;
         if (esp_now_register_recv_cb(onReceive) != ESP_OK) return false;
 
         esp_now_peer_info_t peerInfo = {};
         memcpy(peerInfo.peer_addr, BROADCAST_MAC_ADDRESS, 6);
         peerInfo.channel = CHANNEL;
         peerInfo.encrypt = false;
         if (esp_now_add_peer(&peerInfo) != ESP_OK) return false;
 
         THIS_ADDRESS = address;
         return true;
     }
 
     void end() {
         esp_now_deinit();
     }
 
     bool sendPacket(const NexusPacket &packet) {
         if (packet.size() > ESP_NOW_MAX_DATA_LEN) return false;
         uint8_t buffer[packet.size()];
         memcpy(buffer, &packet, packet.size());
         return esp_now_send(BROADCAST_MAC_ADDRESS, buffer, packet.size()) == ESP_OK;
     }
 
     bool sendData(uint16_t command, uint8_t length, uint8_t data[], const NexusAddress &destination) {
         NexusPacket packet(THIS_ADDRESS, destination, randomSequenceNum(), command, length, data);
         return sendPacket(packet);
     }
 
     bool sendToDevice(uint16_t command, uint8_t length, uint8_t data[], uint8_t deviceID) {
         return sendData(command, length, data, NexusAddress(getProjectID(), 255, deviceID));
     }
 
     bool sendToGroup(uint16_t command, uint8_t length, uint8_t data[], uint8_t groupID) {
         return sendData(command, length, data, NexusAddress(getProjectID(), calcGroupMask(groupID), 255));
     }
 
     bool readPacket(NexusPacket &packet) {
         return incomingBuffer.dequeue(packet);
     }
 
     int available() {
         return incomingBuffer.size();
     }
 
     void scan() {
         shouldScan = true;
     }
 
     void loop() {
         uint32_t now = millis();
         // Send queued outbound packets
         while (outgoingPackets.size() > 0) {
             auto pkt = outgoingPackets[0];
             sendPacket(pkt);
             outgoingPackets.remove(0);
         }
         // Periodic network scan
         if (now - lastScan >= NEXUS_SCAN_INTERVAL) {
             if (shouldScan) {
                 shouldScan = false;
                 isScanComplete = false;
                 lastScan = now;
                 scanSeq = randomSequenceNum();
                 scanResults.clear();
                 sendPacket(NexusPacket(THIS_ADDRESS, NexusAddress(getProjectID(), 255, 255), scanSeq, NEXUS_COMMAND_SCAN, 0, nullptr));
             } else if (!isScanComplete) {
                 // Process scan results
                 for (size_t i = 0; i < scanResults.size(); ++i) {
                     auto dev = scanResults[i];
                     if (!devices.contains(dev)) {
                         devices.addend(dev);
                         if (onDeviceConnected) onDeviceConnected(dev);
                     }
                 }
                 // Handle disconnections
                 for (size_t i = 0; i < devices.size(); ++i) {
                     auto dev = devices[i];
                     if (!scanResults.contains(dev)) {
                         devices.remove(i);
                         --i;
                         if (onDeviceDisconnected) onDeviceDisconnected(dev);
                     }
                 }
                 if (onScanComplete) onScanComplete();
                 isScanComplete = true;
             }
         }
     }
 }
 
 // --------------------- onReceive ---------------------
 /**
  * @brief ESP-NOW receive callback: decodes NexusPacket and dispatches.
  */
 void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
     NexusPacket packet;
     memcpy(&packet, data, len);
     if (packet.version != NEXUS_VERSION) return;
 
     bool toThis = (packet.destination.projectID == Nexus::THIS_ADDRESS.projectID)
                 && (packet.destination.groups & Nexus::THIS_ADDRESS.groups)
                 && (packet.destination.deviceID == Nexus::THIS_ADDRESS.deviceID || packet.destination.deviceID == 255);
     if (!toThis) return;
 
     if (packet.command == NEXUS_COMMAND_SCAN) {
         if (packet.length == 1) {
             if (Nexus::scanSeq == --packet.sequenceNum && !Nexus::isScanComplete && !Nexus::scanResults.contains(packet.source)) {
                 Nexus::scanResults.addend(packet.source);
                 if (Nexus::onDeviceConnected) Nexus::onDeviceConnected(packet.source);
             }
         } else if (packet.length == 0) {
             if (Nexus::onThisScanned && !Nexus::onThisScanned(packet.source)) return;
             Nexus::outgoingPackets.addend(NexusPacket(Nexus::THIS_ADDRESS, packet.source, ++packet.sequenceNum, NEXUS_COMMAND_SCAN, 1, nullptr));
         }
     } else {
         if (Nexus::onPacketReceived) {
             Nexus::onPacketReceived(packet);
         } else {
             Nexus::incomingBuffer.enqueue(packet);
         }
     }
 } 