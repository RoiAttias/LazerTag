#include "Components/Nexus/Nexus.hpp"

void onPeerConnect(const MacAddress &peer) {
    Serial.println("Synchronized with: " + peer.toString());
}

void onPeerDisconnect(const MacAddress &peer, uint8_t error) {
    Serial.println("Disconnected from: " + peer.toString() + " with error: " + String(error));
}

bool onThisScanned(const MacAddress &who) {
    Serial.println("Scanned by: " + who.toString());
    return true;
}

void onScanComplete() {
    Serial.print("Devices found: ");
    Serial.println(Nexus::scanResults.size());

    for (size_t i = 0; i < Nexus::scanResults.size(); i++) {
        MacAddress mac = Nexus::scanResults[i];
        Serial.print("Found device: ");
        Serial.println(mac.toString());
    }

    if (Nexus::scanResults.size() > 0)
        Nexus::syncronize(Nexus::scanResults[0]);
}


void test_setup() {
    Serial.begin(115200);

    Nexus::OnPeerConnect = onPeerConnect;
    Nexus::onPeerDisconnect = onPeerDisconnect;
    Nexus::onThisScanned = onThisScanned;
    Nexus::onScanComplete = onScanComplete;

    Nexus::shouldScan = false;

    if(Nexus::begin(0)) {
        Serial.println("Nexus module initialized successfully!");
    } else {
        Serial.println("Failed to initialize Nexus module!");
    }

}

void test_loop() {
    Nexus::loop();

    if (Nexus::peers.size() > 0) {
        String message = "Hello";
        uint8_t data[5];
        message.getBytes(data, 5);
        Nexus::sendData(data, uint8_t(5), Nexus::peers[0]);
        Serial.println(Nexus::blacklist.size());
        delay(100);
    }

    if (Nexus::available() > 0) {
        NexusPacket packet(0, 0, 0, 0, nullptr);
        Nexus::readPacket(packet);
        Serial.println(packet.toString());
    }
}
