#include <Arduino.h>

#include "Constants.h"
//#include "Utilities\HyperList.hpp"
#include "Subsystems\TGUI\TGUI.hpp"
#include "Components\IRremoteESP32\IRremoteESP32.hpp"

IRreceiver recv(18);

void setup() {
  recv.init();
  Serial.begin(115200);  // Initialize Serial for output
}

void loop() {
  if(recv.available())
  {
    Serial.println(recv.read(),HEX);
  }
}