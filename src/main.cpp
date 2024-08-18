<<<<<<< HEAD
#include "Arduino.h"
#include "Subsystems\IRremoteESP32\IRremoteESP32.hpp"

IRreceiver irReceiver(18, true, 200);  // Create an IRReceiver object (pin 18, invert signal, threshold 200)
=======
#include <Arduino.h>

#include "Constants.h"
#include "Utilities\HyperList.hpp"
//#include "Subsystems\TGUI\TGUI.hpp"
#include "Subsystems\IRremoteESP32\IRremoteESP32.hpp"

IRReceiver irReceiver(14, 200, false); // Pin 14, threshold 200, no invert
>>>>>>> parent of 143d029 (Problem in ISR attaching inside the class.)

// Function to handle received data
void handleReceivedData(unsigned long data) {
  Serial.print("Received data: ");
  Serial.println(data, HEX);  // Print the received data in hexadecimal format
}

void setup() {
<<<<<<< HEAD
  Serial.begin(115200);  // Initialize Serial for output

  // Set the custom event handler for when data is received
  irReceiver.OnReceive_setHandler(handleReceivedData);
  // Enable the OnReceive event handler
=======
  Serial.begin(115200);
  irReceiver.OnReceive_setHandler(onReceive);
>>>>>>> parent of 143d029 (Problem in ISR attaching inside the class.)
  irReceiver.OnReceive_enable(true);
}

void loop() {
<<<<<<< HEAD
  // Optionally, you can use the available() and read() methods to check and retrieve data manually
  if (irReceiver.available()) {
    unsigned long data = irReceiver.read();
    Serial.print("Manual read: ");
    Serial.println(data, HEX);
  }

  // The rest of your loop code here
}
=======
  // Optionally handle received data here if needed
  if (irReceiver.available()) {
    unsigned long result = irReceiver.read();
    Serial.print("Received NEC command: 0x");
    Serial.println(result, HEX);
  }
}
>>>>>>> parent of 143d029 (Problem in ISR attaching inside the class.)
