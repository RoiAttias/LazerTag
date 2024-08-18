#include <Arduino.h>
/**
#include "Constants.h"
#include "Utilities\HyperList.hpp"
//#include "Subsystems\TGUI\TGUI.hpp"*/
#include "Subsystems\IRremoteESP32\IRremoteESP32.hpp"

IRReceiver irReceiver(18,true); // Pin 14, threshold 200, invert

void onReceive(unsigned long data) {
  Serial.print("Received NEC command: 0x");
  Serial.println(data, HEX);
}
void fun(void *arg){
  int *x = static_cast<int *>(arg);
  Serial.println("Success!");
}
int num = 0;
void setup() {
  Serial.begin(115200);/* 
  irReceiver.OnReceive_setHandler(onReceive);
  irReceiver.OnReceive_enable(true);*/
  /*
  esp_err_t err = gpio_install_isr_service(0);
    if (err != ESP_OK) {
        Serial.printf("Failed to install ISR service: %d\n", err);
    } else {
        Serial.println("ISR service installed successfully.");
    }
    */
}

void loop() {/*
  // Optionally handle received data here if needed
  if (irReceiver.available()) {
    unsigned long result = irReceiver.read();
    Serial.print("Received NEC command: 0x");
    Serial.println(result, HEX);
  }*/
}