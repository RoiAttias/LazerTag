#include <Arduino.h>

#include "Constants.h"
#include "Utilities\HyperList.hpp"
//#include "Subsystems\TGUI\TGUI.hpp"
#include "Subsystems\IRremoteESP32\IRremoteESP32.hpp"

IRReceiver irReceiver(18, true, 200);

// Function to handle received data
void handleReceivedData(unsigned long data) {
  Serial.print("Received data: ");
  Serial.println(data, HEX);  // Print the received data in hexadecimal format
}

void setup() {
  Serial.begin(115200);  // Initialize Serial for output

  // Set the custom event handler for when data is received
  irReceiver.OnReceive_setHandler(handleReceivedData);
  // Enable the OnReceive event handler
  irReceiver.OnReceive_enable(true);
}

void loop() {
  // Optionally, you can use the available() and read() methods to check and retrieve data manually
  if (irReceiver.available()) {
    unsigned long data = irReceiver.read();
    Serial.print("Manual read: ");
    Serial.println(data, HEX);
  }

  // The rest of your loop code here
}