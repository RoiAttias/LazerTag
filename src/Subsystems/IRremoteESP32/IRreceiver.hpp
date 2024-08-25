#ifndef IRRECEIVER_HPP
#define IRRECEIVER_HPP

#include "IRremoteESP32.hpp"

enum NEC_STAGE
{
  headerMark,
  headerSpace,
  bitMark,
  bitSpace,
};

bool compare (unsigned long value, unsigned long valToCompare, unsigned long epsilon)
{
  return value >= (valToCompare - epsilon) && value <= (valToCompare + epsilon);
}

class IRreceiver
{
private:
  int recvPin;
  bool invert;

  unsigned long data;
  bool dataAvailable;

  unsigned long dataRead;
  unsigned long lastTime;
  unsigned long currentTime;
  unsigned long duration;
  int bitCount;
  NEC_STAGE stage;

public:
  IRreceiver(int pin)//, void (*interrupt4decode)(void))//, bool invertedSignal)
  {
    recvPin = pin;
    //invert = invertedSignal;
    dataRead = 0UL;
    data = 0UL;
    bitCount = 0;
    dataAvailable = false;

    pinMode(recvPin, INPUT_PULLUP);
    //attachInterrupt(digitalPinToInterrupt(recvPin),interrupt4decode, CHANGE);
    lastTime = micros();

    stage = headerMark;

    // invert ? FALLING : RISING;
  }

  ~IRreceiver()
  {
    detachInterrupt(digitalPinToInterrupt(recvPin));
  }

  bool available()
  {
    return dataAvailable;
  }

  unsigned long read()
  {
    dataAvailable = false;
    return data;
  }

  NEC_STAGE getstageNEC()
  {
    return stage;
  }

  void decodeNEC()
  {
    if(dataAvailable)
      return;
    
    currentTime = micros();
    duration = currentTime - lastTime;

    if(duration < NEC_MAX_EXPECTED_BOUNCES)
      return;

    lastTime = currentTime;

    switch (stage)
    {
    case headerMark:
      if (compare(duration, NEC_HEADER_MARK, NEC_THRESHOLD))
      {
        stage = headerSpace;
      }
      else
      {
        stage = headerMark; // Reset to header mark stage if not matched
      }
      break;

    case headerSpace:
      if (compare(duration, NEC_HEADER_SPACE, NEC_THRESHOLD))
      {
        stage = bitMark;
        bitCount = 0;
        dataRead = 0;
      }
      else
      {
        stage = headerMark; // Reset to header mark stage if not matched
      }
      break;

    case bitMark:
      if (compare(duration, NEC_BIT_MARK, NEC_THRESHOLD))
      {
        stage = bitSpace;
      }
      else
      {
        stage = headerMark; // Reset to header mark stage if not matched
      }
      break;

    case bitSpace:
      if (compare(duration, NEC_ONE_SPACE, NEC_THRESHOLD))
      {
        dataRead = (dataRead << 1) | 1; // Received a '1'
        bitCount++;
      }
      else if (compare(duration, NEC_ZERO_SPACE, NEC_THRESHOLD))
      {
        dataRead = (dataRead << 1) | 0; // Received a '0'
        bitCount++;
      }
      else
      {
        stage = headerMark; // Reset to header mark stage if not matched
      }

      if (bitCount == NEC_BITS)
      { // Check if we have received all bits
        data = dataRead;
        dataAvailable = true;
        stage = headerMark; // Reset for next signal
      }
      else
      {
        stage = bitMark; // Go back to wait for next bit mark
      }
      break;
    }
    //Serial.println(duration);
  }
};

#endif // IRRECEIVER_HPP