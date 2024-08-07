#include <Arduino.h>

#include "Constants.h"
#include "Utilities\HyperList.hpp"
//#include "Subsystems\TGUI\TGUI.hpp"
#include "Subsystems\IRremoteESP32\IRremoteESP32.hpp"

IRReceiver irReceiver(14, 200, false); // Pin 14, threshold 200, no invert

void onReceive(unsigned long data) {
  Serial.print("Received NEC command: 0x");
  Serial.println(data, HEX);
}

void setup() {
  Serial.begin(115200);
  irReceiver.OnReceive_setHandler(onReceive);
  irReceiver.OnReceive_enable(true);
}

void loop() {
  // Optionally handle received data here if needed
  if (irReceiver.available()) {
    unsigned long result = irReceiver.read();
    Serial.print("Received NEC command: 0x");
    Serial.println(result, HEX);
  }
}