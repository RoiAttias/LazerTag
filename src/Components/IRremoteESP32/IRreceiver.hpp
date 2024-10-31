#ifndef IRRECEIVER_HPP
#define IRRECEIVER_HPP

#include "IRremoteESP32.hpp"
#include "Utilities\HyperMap.hpp"

typedef void (*voidFuncPtr)(void);
typedef void (*voidFuncPtrArg)(void*);

class IRreceiver;
void registerReceiver(int recvPin, IRreceiver *recvPtr);
void IRAM_ATTR decodeHandler(void *arg);

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
  NEC_STAGE nec_stage;

public:
  IRreceiver(int pin)
  {
    recvPin = pin;
    //invert = invertedSignal;
    dataRead = 0UL;
    data = 0UL;
    bitCount = 0;
    dataAvailable = false;
    
    pinMode(recvPin, INPUT_PULLUP);
  }

  ~IRreceiver()
  {
    detachInterrupt(digitalPinToInterrupt(recvPin));
  }

  void init()
  {
    registerReceiver(recvPin, this);
    
    lastTime = micros();
    nec_stage = headerMark;
  }

  int getPin()
  {
    return recvPin;
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

  NEC_STAGE getNEC_STAGE()
  {
    return nec_stage;
  }

  void decodeNEC()
  {
    if(dataAvailable)
      return;
    
    currentTime = micros();
    duration = currentTime - lastTime;
    Serial.println(duration);
    if(duration < NEC_BOUNCE_STOP_FILTER)
      return;

    lastTime = currentTime;

    switch (nec_stage)
    {
    case headerMark:
      if (compare(duration, NEC_HEADER_MARK, NEC_THRESHOLD))
      {
        nec_stage = headerSpace;
      }
      else
      {
        nec_stage = headerMark; // Reset to header mark nec_stage if not matched
      }
      break;

    case headerSpace:
      if (compare(duration, NEC_HEADER_SPACE, NEC_THRESHOLD))
      {
        nec_stage = bitMark;
        bitCount = 0;
        dataRead = 0;
      }
      else
      {
        nec_stage = headerMark; // Reset to header mark nec_stage if not matched
      }
      break;

    case bitMark:
      if (compare(duration, NEC_BIT_MARK, NEC_THRESHOLD))
      {
        nec_stage = bitSpace;
      }
      else
      {
        nec_stage = headerMark; // Reset to header mark nec_stage if not matched
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
        nec_stage = headerMark; // Reset to header mark nec_stage if not matched
      }

      if (bitCount == NEC_BITS)
      { // Check if we have received all bits
        data = dataRead;
        dataAvailable = true;
        nec_stage = headerMark; // Reset for next signal
      }
      else
      {
        nec_stage = bitMark; // Go back to wait for next bit mark
      }
      break;
    }
  }
};

HyperMap<int,IRreceiver*> receiverInfoList;

void registerReceiver(int recvPin, IRreceiver *recvPtr)
{
  receiverInfoList.put(recvPin, recvPtr);
  void* recvPinValue = reinterpret_cast<void*>(recvPin);
  attachInterruptArg(digitalPinToInterrupt(recvPin),decodeHandler, recvPinValue, CHANGE);
}

void IRAM_ATTR decodeHandler(void *arg)
{
  int recvPin = reinterpret_cast<int>(arg);
  IRreceiver *irrptr = receiverInfoList.get(recvPin);
  irrptr->decodeNEC();
}

#endif // IRRECEIVER_HPP