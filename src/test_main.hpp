#include "Components/Nexus/Nexus.hpp"

bool onThisScanned(const MacAddress &who) {
    Serial.println("Scanned by: " + who.toString());
    return true;
}

void onScanComplete() {
    Serial.println("Scan complete!");
    Serial.print("Devices found: ");
    Serial.println(Nexus::scanResults.size());

    for (size_t i = 0; i < Nexus::scanResults.size(); i++) {
        MacAddress mac = Nexus::scanResults[i];
        Serial.print("Found device: ");
        Serial.println(mac.toString());
    }
}


void test_setup() {
    Serial.begin(115200);

    Nexus::onThisScanned = onThisScanned;
    Nexus::onScanComplete = onScanComplete;

    if(Nexus::begin(0)) {
        Serial.println("Nexus module initialized successfully!");
    } else {
        Serial.println("Failed to initialize Nexus module!");
    }

    Serial.println("Address: " + Nexus::THIS_ADDRESS.toString());
}

void test_loop() {
    Nexus::loop();
}
