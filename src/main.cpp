#include <Arduino.h>
/*
#include "Constants.h"
//#include "Utilities\HyperList.hpp"
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

enum NEC_STAGE {
  headerMark, headerSpace,
  bitMark, bitSpace
};

class IRreceiver
{
private:
  int recvPin;
  bool invert;
  unsigned long lastTime;
  unsigned long data;
  int bitCount;
  int threshold;
  bool dataAvailable;
  NEC_STAGE stage = headerMark;
};
const int recvPin = 18;
unsigned long lastTime = 0;
unsigned long data = 0;
int bitCount = 0;
bool dataAvailable = 0;
NEC_STAGE stage = headerMark;

unsigned long duration;

bool doStuff = false;

bool compare (unsigned long value, unsigned long valToCompare, unsigned long epsilon) {
  return value >= (valToCompare - epsilon) && value <= (valToCompare + epsilon);
}

void IRAM_ATTR decodeNEC() {
  unsigned long currentTime = micros();
  duration = currentTime - lastTime;
  lastTime = currentTime;
  doStuff = true;
/*
  switch (stage) {
    case headerMark:
      if (compare(duration, NEC_HEADER_MARK, NEC_THRESHOLD)) {
        stage = headerSpace;
      } else {
        stage = headerMark; // Reset to header mark stage if not matched
      }
      break;

    case headerSpace:
      if (compare(duration, NEC_HEADER_SPACE, NEC_THRESHOLD)) {
        stage = bitMark;
        bitCount = 0;
        data = 0;
      } else {
        stage = headerMark; // Reset to header mark stage if not matched
      }
      break;

    case bitMark:
      if (compare(duration, NEC_BIT_MARK, NEC_THRESHOLD)) {
        stage = bitSpace;
      } else {
        stage = headerMark; // Reset to header mark stage if not matched
      }
      break;

    case bitSpace:
      if (compare(duration, NEC_ONE_SPACE, NEC_THRESHOLD)) {
        data = (data << 1) | 1; // Received a '1'
        bitCount++;
      } else if (compare(duration, NEC_ZERO_SPACE, NEC_THRESHOLD)) {
        data = (data << 1) | 0; // Received a '0'
        bitCount++;
      } else {
        stage = headerMark; // Reset to header mark stage if not matched
      }

      if (bitCount == NEC_BITS) { // Check if we have received all bits
        dataAvailable = true;
        stage = headerMark; // Reset for next signal
        handleReceivedData(data);
      } else {
        stage = bitMark; // Go back to wait for next bit mark
      }
      break;
  }*/
}

void handleReceivedData(unsigned long data) {
  Serial.print("Received data: ");
  Serial.println(data, HEX);  // Print the received data in hexadecimal format
}

void setup() {
  Serial.begin(115200);  // Initialize Serial for output
  pinMode(recvPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(recvPin), decodeNEC, CHANGE);
  lastTime = micros();
}

void loop() {
  if(doStuff)
  {
    //Serial.print("change: ");
    Serial.println(duration);
    doStuff = false;
  }
}