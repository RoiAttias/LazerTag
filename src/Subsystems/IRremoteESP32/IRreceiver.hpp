#ifndef IRRECEIVER_HPP
#define IRRECEIVER_HPP

#include <Arduino.h>

#define NEC_BITS 32
#define NEC_HEADER_MARK 9000UL
#define NEC_HEADER_SPACE 4500UL
#define NEC_BIT_MARK 562UL
#define NEC_ONE_SPACE 1687UL
#define NEC_ZERO_SPACE 562UL
#define NEC_THRESHOLD 100UL

enum NEC_STAGE
{
  headerMark,
  headerSpace,
  bitMark,
  bitSpace
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

  unsigned long lastTime;
  unsigned long currentTime;
  unsigned long duration;
  int bitCount;
  NEC_STAGE stage = headerMark;

public:
  IRreceiver(int pin, void (*interrupt4decode)(void))//, bool invertedSignal)
  {
    recvPin = pin;
    //invert = invertedSignal;
    data = 0UL;
    bitCount = 0;
    dataAvailable = 0;

    pinMode(recvPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(recvPin),interrupt4decode, CHANGE);
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

  void decodeNEC()
  {
    currentTime = micros();
    duration = currentTime - lastTime;
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
        data = 0;
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
        data = (data << 1) | 1; // Received a '1'
        bitCount++;
      }
      else if (compare(duration, NEC_ZERO_SPACE, NEC_THRESHOLD))
      {
        data = (data << 1) | 0; // Received a '0'
        bitCount++;
      }
      else
      {
        stage = headerMark; // Reset to header mark stage if not matched
      }

      if (bitCount == NEC_BITS)
      { // Check if we have received all bits
        dataAvailable = true;
        stage = headerMark; // Reset for next signal
      }
      else
      {
        stage = bitMark; // Go back to wait for next bit mark
      }
      break;
    }
  }
};

#endif // IRRECEIVER_HPP