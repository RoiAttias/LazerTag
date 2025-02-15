#include <esp_now.h>
#include <WiFi.h>

uint8_t peerMac[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};  // Change this to the MAC of Device 2

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Send status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    Serial.print("Received from MAC: ");
    for (int i = 0; i < 6; i++) Serial.printf("%02X:", mac[i]);
    Serial.println();
    
    Serial.print("Message: ");
    Serial.write(incomingData, len);
    Serial.println();
}

void test_setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW Init Failed");
        return;
    }

    esp_now_register_send_cb(onDataSent);
    esp_now_register_recv_cb(onDataRecv);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, peerMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
    }
}

void test_loop() {
    if (Serial.available()) {
        String message = Serial.readStringUntil('\n');
        message.trim();
        esp_now_send(peerMac, (uint8_t *)message.c_str(), message.length());
    }
}
