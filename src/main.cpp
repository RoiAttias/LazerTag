#include <Arduino.h>
/*
#include "Constants.h"
#include "Utilities\HyperList.hpp"
//#include "Subsystems\TGUI\TGUI.hpp"
#include "Subsystems\IRremoteESP32\IRremoteESP32.hpp"

IRReceiver irReceiver(18, true, 200);
*/
// Function to handle received data

#define NEC_BITS 32
#define NEC_HEADER_MARK 9000
#define NEC_HEADER_SPACE 4500
#define NEC_BIT_MARK 560
#define NEC_ONE_SPACE 1690
#define NEC_ZERO_SPACE 560
#define NEC_THRESHOLD 50
#define INVERT true

const int recvPin = 18;
unsigned long lastTime = 0;
unsigned long data = 0;
int bitCount = 0;
bool dataAvailable = 0;

unsigned long duration;

bool compare (unsigned long value, unsigned long valToCompare, unsigned long epsilon) {
  return value >= (valToCompare - epsilon) && value <= (valToCompare + epsilon);
}

void IRAM_ATTR isr() {
  unsigned long currentTime = micros();
	duration = currentTime - lastTime;
  lastTime = currentTime;
  bitCount++;
}

void decodeNEC() {
  
}

void handleReceivedData(unsigned long data) {
  Serial.print("Received data: ");
  Serial.println(data, HEX);  // Print the received data in hexadecimal format
}

void setup() {
  Serial.begin(115200);  // Initialize Serial for output
  pinMode(recvPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(recvPin), isr, FALLING);
  lastTime = micros();
}

void loop() {
  if(bitCount > 0)
  {
    Serial.print("change: ");
    Serial.println(duration);
    bitCount--;
  }
}