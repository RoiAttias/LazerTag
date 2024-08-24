/*

#include <Arduino.h>

#include "Constants.h"
//#include "Utilities\HyperList.hpp"
//#include "Subsystems\TGUI\TGUI.hpp"
#include "Subsystems\IRremoteESP32\IRremoteESP32.hpp"

IRsender irSender(4, 0, 38000, false); // Pin 4, channel 0, 38kHz frequency, no invert

void setup() {
  Serial.begin(115200);  // Initialize Serial for output
}

void loop() {
  // Send an NEC command
  unsigned long command = 0x20DF10EF; // Example command
  Serial.print("Sending NEC command: 0x");
  Serial.println(command, HEX);
  irSender.sendNEC(command, 32);
  delay(2000); // Wait 2 seconds before sending again
}
*/