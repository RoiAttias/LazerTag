#include "Nexus.hpp"
#include <string.h>

// ---------------------
// NexusAddress methods
// ---------------------
NexusAddress::NexusAddress(uint8_t projectID, uint8_t groups, uint8_t deviceID)
    : projectID(projectID), groups(groups), deviceID(deviceID) {}

void NexusAddress::toBuffer(uint8_t buffer[]) {
    buffer[0] = projectID;
    buffer[1] = groups;
    buffer[2] = deviceID;
}

String NexusAddress::toString() const {
    return String(projectID) + "." + String(groups) + "." + String(deviceID);
}

bool NexusAddress::operator==(const NexusAddress &other) const {
    return projectID == other.projectID && groups == other.groups && deviceID == other.deviceID;
}

bool NexusAddress::operator!=(const NexusAddress &other) const {
    return !(*this == other);
}

// ---------------------
// NexusPacket methods
// ---------------------
NexusPacket::NexusPacket(NexusAddress source, NexusAddress destination,
                uint16_t sequenceNum, uint16_t command, uint8_t length, const uint8_t payload[])
    : version(NEXUS_VERSION), source(source), destination(destination),
      sequenceNum(sequenceNum), command(command), length(length) {
    if(payload != nullptr) {
        memcpy(this->payload, payload, length);
    }
}

size_t NexusPacket::size() const {
    return length + NEXUS_HEADER_SIZE;
}

String NexusPacket::toString() const {
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

// ---------------------
// Nexus namespace definitions
// ---------------------
namespace Nexus {
    int CHANNEL = 0;
    const uint8_t BROADCAST_MAC_ADDRESS[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    void (* onDeviceConnected)(const NexusAddress &who) = nullptr;
    void (* onDeviceDisconnected)(const NexusAddress &who) = nullptr;
    void (* onScanComplete)() = nullptr;
    bool (* onThisScanned)(const NexusAddress &who) = nullptr;
    void (* onPacketReceived)(const NexusPacket &packet) = nullptr;

    uint32_t lastScan = 0;
    uint16_t scanSeq = 0;
    bool isScanComplete = false;
    bool shouldScan = false;

    NexusAddress THIS_ADDRESS(0, 0, 0);
    HyperList<NexusAddress> devices;
    HyperList<NexusAddress> scanResults;
    PacketBuffer<NexusPacket> incomingBuffer(NEXUS_BUFFER_SIZE);
    HyperList<NexusPacket> outgoingPackets;

    void setAddress(uint8_t projectID, uint8_t groups, uint8_t deviceID) {
        THIS_ADDRESS.projectID = projectID;
        THIS_ADDRESS.groups = groups;
        THIS_ADDRESS.deviceID = deviceID;
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
        return random(0, 65535);
    }

    bool begin(const NexusAddress &address) {
        WiFi.mode(WIFI_STA);
        if (esp_now_init() != ESP_OK) return false;
        if (esp_now_register_recv_cb(onReceive) != ESP_OK) return false;

        esp_now_peer_info_t peerInfo = {};
        memcpy(peerInfo.peer_addr, BROADCAST_MAC_ADDRESS, 6);
        peerInfo.channel = CHANNEL;
        peerInfo.encrypt = false;
        if(esp_now_add_peer(&peerInfo) != ESP_OK) return false;

        THIS_ADDRESS = address;
        return true;
    }

    void end() {
        esp_now_deinit();
    }

    bool sendPacket(const NexusPacket &packet) {
        if (packet.size() > ESP_NOW_MAX_DATA_LEN) return false;
        uint8_t data[packet.size()];
        memcpy(data, &packet, packet.size());
        esp_err_t result = esp_now_send(BROADCAST_MAC_ADDRESS, data, packet.size());
        return result == ESP_OK;
    }

    bool sendData(uint16_t command, uint8_t length, uint8_t data[], const NexusAddress &destination) {
        NexusPacket packet(THIS_ADDRESS, destination, randomSequenceNum(), command, length, data);
        return sendPacket(packet);
    }

    bool sendToDevice(uint16_t command, uint8_t length, uint8_t data[], const uint8_t deviceID) {
        return sendData(command, length, data, NexusAddress(getProjectID(), 255, deviceID));
    }

    bool sendToGroup(uint16_t command, uint8_t length, uint8_t data[], const uint8_t groupID) {
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
        while (outgoingPackets.size() > 0) {
            auto packet = outgoingPackets[0];
            sendPacket(packet);
            outgoingPackets.remove(0);
        }

        if (now - lastScan >= NEXUS_SCAN_INTERVAL) {
            // Check if a the scan flag is set
            if (shouldScan) {
                Serial.println("Scanning...");
                shouldScan = false;
                isScanComplete = false;
                lastScan = now;
                scanSeq = randomSequenceNum();
                scanResults.clear();
                sendPacket(NexusPacket(THIS_ADDRESS, NexusAddress(getProjectID(), 255, 255), scanSeq, NEXUS_COMMAND_SCAN, 0, nullptr));
            } else if (!isScanComplete) {
                // If scan timeout is reached, begin processing the scan results, then mark the scan as complete

                // Process newly scanned devices
                for (size_t i = 0; i < scanResults.size(); i++) {
                    auto device = scanResults[i];
                    if (!devices.contains(device)) {
                        devices.addend(device);
                        if (onDeviceConnected != nullptr) {
                            onDeviceConnected(device);
                        }
                    }
                }
                // Process disconnected devices
                for (size_t i = 0; i < devices.size(); i++) {
                    auto device = devices[i];
                    if (!scanResults.contains(device)) {
                        devices.remove(i);
                        if (onDeviceDisconnected != nullptr) {
                            onDeviceDisconnected(device);
                        }
                    }
                }

                // Call the scan complete callback if set
                if (onScanComplete != nullptr) {
                    onScanComplete();
                }

                // Mark the scan as complete
                isScanComplete = true;
            }
        }
    }
}

// ---------------------
// onReceive definition
// ---------------------
void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
    NexusPacket packet;
    memcpy(&packet, data, len);
    if (packet.version != NEXUS_VERSION) return;

    if ((packet.destination.projectID == Nexus::THIS_ADDRESS.projectID) &&
        (packet.destination.groups & Nexus::THIS_ADDRESS.groups) &&
        (packet.destination.deviceID == Nexus::THIS_ADDRESS.deviceID || packet.destination.deviceID == 255)) {
        if (packet.command == NEXUS_COMMAND_SCAN) {
            if (packet.length == 1) {
                if (Nexus::scanSeq == --packet.sequenceNum) {
                    if (!Nexus::isScanComplete && !Nexus::scanResults.contains(packet.source)) {
                        Serial.println("Found device: " + packet.source.toString());
                        Nexus::scanResults.addend(packet.source);
                        Serial.println("Added device: " + packet.source.toString());
                    }
                }
            } else if (packet.length == 0) {
                if (Nexus::onThisScanned != nullptr) {
                    if (!Nexus::onThisScanned(packet.source)) {
                        return;
                    }
                }
                Nexus::outgoingPackets.addend(NexusPacket(Nexus::THIS_ADDRESS, packet.source, ++packet.sequenceNum, NEXUS_COMMAND_SCAN, 1, nullptr));
            }
        } else {
            if (Nexus::onPacketReceived != nullptr) {
                Nexus::onPacketReceived(packet);
            } else {
                Nexus::incomingBuffer.enqueue(packet);
            }
        }
    }
}
