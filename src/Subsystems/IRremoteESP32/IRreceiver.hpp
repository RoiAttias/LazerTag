#ifndef IRRECEIVER_HPP
#define IRRECEIVER_HPP

#include <Arduino.h>

#define NEC_BITS 32
#define NEC_HEADER_MARK 9000
#define NEC_HEADER_SPACE 4500
#define NEC_BIT_MARK 560
#define NEC_ONE_SPACE 1690
#define NEC_ZERO_SPACE 560

typedef void (*OnReceiveHandler)(unsigned long data);

class IRReceiver
{
private:
  int recvPin;
  bool invert;
  unsigned long lastTime;
  unsigned long data;
  int bitCount;
  int threshold;
  bool dataAvailable;
  bool onReceiveEnable;
  OnReceiveHandler onReceiveHandler;

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
  IRReceiver(int pin, bool invertSignal = true, int threshold = 200)
      : recvPin(pin), invert(invertSignal), threshold(threshold),
        lastTime(0), data(0), bitCount(0), 
        dataAvailable(false), onReceiveHandler(nullptr), onReceiveEnable(false)
  {
    pinMode(recvPin, INPUT);
    attachInterruptArg(digitalPinToInterrupt(recvPin), ISRWrapper, this, invert ? FALLING : RISING);
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

  void OnReceive_enable(bool enable)
  {
    onReceiveEnable = enable;
  }

  void OnReceive_setHandler(OnReceiveHandler handler)
  {
    onReceiveHandler = handler;
  }

  void OnReceive_execute()
  {
    if (dataAvailable && onReceiveHandler && onReceiveEnable)
    {
      onReceiveHandler(data);
    }
  }
};

#endif // IRRECEIVER_HPP