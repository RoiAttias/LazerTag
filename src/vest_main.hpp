// Includes libraries:
#include "Components/IRremoteESP32/IRremoteESP32.hpp"

void IRAM_ATTR recv_isr();

// Creates receiver object
IRreceiver recv(18,recv_isr);

void IRAM_ATTR recv_isr() {
  recv.decodeNEC();
}

void vest_setup() {
  Serial.begin(115200);  // Initialize Serial for output
  recv.init(); // Initializes the receiver object
}

void vest_loop() {
  // Checks if message decoded and received
  if(recv.available())
  {
    // Prints HEX value of received message
    Serial.println(recv.read().data,HEX);
  }
}