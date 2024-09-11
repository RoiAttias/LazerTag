#include <Arduino.h>

#include "Constants.h"
//#include "Utilities\HyperList.hpp"
//#include "Subsystems\TGUI\TGUI.hpp"
#include "Components\IRremoteESP32\IRremoteESP32.hpp"


//void IRAM_ATTR handleDecoding();

// add channels to handleDecoding to pass it as arg
IRreceiver recv(18);

//,handleDecoding);
/*
void IRAM_ATTR handleDecoding()
{
  recv.decodeNEC();
}*/

void setup() {
  Serial.begin(115200);  // Initialize Serial for output
  //pinMode(recvPin, INPUT_PULLUP);
  attachInterruptArg(digitalPinToInterrupt(18),decodeHandler, reinterpret_cast<void*>(recv.getPin()), CHANGE);
  //lastTime = micros();
}

void loop() {
  if(recv.available())
  {
    Serial.println(recv.read(),HEX);
  }
}