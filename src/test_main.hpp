#include "Components/Nexus/Nexus.hpp"

void onScanComplete(const HyperList<MacAddress> &scanResults) {
    Serial.println("Scan complete!");
    for (size_t i = 0; i < Nexus::scanResults.size(); i++) {
        MacAddress mac = Nexus::scanResults[i];
        Serial.print("Found device: ");
        Serial.println(mac.toString());
    }
}

void test_setup() {
    Serial.begin(115200);

    Nexus::onScanComplete = onScanComplete;

    if(Nexus::begin()) {
        Serial.println("Nexus module initialized successfully!");
    } else {
        Serial.println("Failed to initialize Nexus module!");
    }
}

void test_loop() {
    Nexus::loop();
}
