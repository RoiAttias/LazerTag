/*

#include <Arduino.h>

#include "Constants.h"
//#include "Utilities\HyperList.hpp"
//#include "Subsystems\TGUI\TGUI.hpp"
#include "Subsystems\IRremoteESP32\IRremoteESP32.hpp"


void IRAM_ATTR handleDecoding();
// add channels to handleDecoding to pass it as arg
IRreceiver recv(18,handleDecoding);

void IRAM_ATTR handleDecoding()
{
  recv.decodeNEC();
}

void handleReceivedData(unsigned long data) {
  Serial.print("Received data: ");
  Serial.println(data, HEX);  // Print the received data in hexadecimal format
}

void setup() {
  Serial.begin(115200);  // Initialize Serial for output
  //pinMode(recvPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(recvPin), decodeNEC, CHANGE);
  //lastTime = micros();
}

void loop() {
  if(recv.available())
  {
    Serial.println(recv.read());
  }
}

*/