#ifndef IRRECEIVER_HPP
#define IRRECEIVER_HPP

#include <Arduino.h>

#define NEC_BITS 32
#define NEC_HEADER_MARK 9000UL
#define NEC_HEADER_SPACE 4500UL
#define NEC_BIT_MARK 560UL
#define NEC_ONE_SPACE 1690UL
#define NEC_ZERO_SPACE 560UL
#define NEC_THRESHOLD 50

class IRreceiver
{
private:
  int recvPin;
  bool invert;
  unsigned long lastTime;
  unsigned long data;
  int bitCount;
  bool dataAvailable;
  NEC_STAGE stage = headerMark;

  static void IRAM_ATTR ISRWrapper(void *arg)
  {
    IRReceiver *self = static_cast<IRReceiver *>(arg);
    self->decodeNec();
  }

  void decodeNec()
  {
    unsigned long time = micros();
    unsigned long duration = time - lastTime;
    lastTime = time;

    //if

    if (duration >= (NEC_HEADER_MARK - threshold) && duration <= (NEC_HEADER_MARK + threshold))
    {
      // Detected header mark
      bitCount = 0;
      data = 0;
      return;
    }

    if (bitCount >= NEC_BITS)
    {
      // Already received full data
      return;
    }

    if (duration >= (NEC_BIT_MARK - threshold) && duration <= (NEC_BIT_MARK + threshold))
    {
      // Detected bit mark
      if (invert)
      {
        duration = digitalRead(recvPin) ? duration : 0;
      }
      if (duration >= (NEC_ONE_SPACE - threshold) && duration <= (NEC_ONE_SPACE + threshold))
      {
        data = (data << 1) | 1;
      }
      else if (duration >= (NEC_ZERO_SPACE - threshold) && duration <= (NEC_ZERO_SPACE + threshold))
      {
        data = (data << 1);
      }
      bitCount++;

      if (bitCount == NEC_BITS)
      {
        dataAvailable = true;
        OnReceive_execute();
      }
    }
  }

public:
  IRReceiver::IRReceiver(int pin, bool invert, unsigned long threshold)
    : _pin(pin), _invert(invert), _threshold(threshold), _lastTime(0), 
      _data(0), _bitCount(0), _dataAvailable(false), _stage(headerMark) {

    pinMode(recvPin, INPUT);
    
    //attachInterruptArg(digitalPinToInterrupt(recvPin), ISRWrapper, this, invert ? FALLING : RISING);
    
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


};

#endif // IRRECEIVER_HPP