#include <Arduino.h>

#include "Constants.h"
#include "Utilities\HyperList.hpp"
//#include "Subsystems\TGUI\TGUI.hpp"
#include "Subsystems\IRremoteESP32\IRremoteESP32.hpp"

IRSender irSender(12, 0, 38000, false); // Pin 12, channel 0, 38kHz frequency, no invert

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Send an NEC command
  unsigned long command = 0x20DF10EF; // Example command
  Serial.print("Sending NEC command: 0x");
  Serial.println(command, HEX);
  irSender.sendNEC(command, 32);
  delay(5000); // Wait 5 seconds before sending again
}
