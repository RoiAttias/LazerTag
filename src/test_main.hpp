#include "Components/Nexus/Nexus.hpp"

// Callback function for when a device is connected
void onDeviceConnected(const NexusAddress &who)
{
    Serial.println("Device connected: " + who.toString());
}

// Callback function for when a device is disconnected
void onDeviceDisconnected(const NexusAddress &who)
{
    Serial.println("Device disconnected: " + who.toString());
}

// Callback function for when this device is scanned
bool onThisScanned(const NexusAddress &who) {
    Serial.println("Scanned by: " + who.toString());
    return true;
}

// Callback function for when a scan is complete
void onScanComplete() {
    Serial.print("Devices found: ");
    Serial.println(Nexus::scanResults.size());

    for (size_t i = 0; i < Nexus::scanResults.size(); i++) {
        auto addr = Nexus::scanResults[i];
        Serial.print("Found device: ");
        Serial.println(addr.toString());
    }
}

// Callback function for when a packet is received
void onPacketReceived(const NexusPacket &packet) {
    Serial.println(packet.toString());
}





void test_setup() {
    Serial.begin(115200);

    Nexus::onDeviceConnected = onDeviceConnected;
    Nexus::onDeviceDisconnected = onDeviceDisconnected;
    Nexus::onScanComplete = onScanComplete;
    Nexus::onThisScanned = onThisScanned;
    Nexus::onPacketReceived = onPacketReceived;

    if(Nexus::begin(NexusAddress(1, 1, 1))) {
        Serial.println("Nexus module initialized successfully!");
    } else {
        Serial.println("Failed to initialize Nexus module!");
    }

}

void test_loop() {
    Nexus::loop();

    Nexus::scan();
    delay(1000);
}
