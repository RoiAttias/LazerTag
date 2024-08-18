#ifndef IRRECEIVER_HPP
#define IRRECEIVER_HPP

#include <Arduino.h>

#define NEC_BITS 32
#define NEC_HDR_MARK 9000
#define NEC_HDR_SPACE 4500
#define NEC_BIT_MARK 560
#define NEC_ONE_SPACE 1690
#define NEC_ZERO_SPACE 560

<<<<<<< HEAD
class IRreceiver
{
private:
  int recvPin;
  bool invert;
  volatile unsigned long lastTime;
  volatile unsigned long data;
  volatile int bitCount;
  int threshold;
  volatile bool dataAvailable;
  volatile bool onReceiveEnable;
  void (*onReceiveHandler)(unsigned long data);
  
  static void IRAM_ATTR ISRWrapper(void *arg)
  {
    IRreceiver *self = static_cast<IRreceiver *>(arg);
    self->decodeNec();
  }

  void IRAM_ATTR decodeNec()
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
=======
typedef void (*OnReceiveHandler)(unsigned long data);

class IRReceiver {
  private:
    int recvPin;
    bool invert;
    unsigned long lastTime;
    unsigned long data;
    int bitCount;
    int threshold;
    bool dataAvailable;
    bool enable;
    OnReceiveHandler onReceiveHandler;

    static void IRAM_ATTR ISRWrapper(void *arg) {
      IRReceiver* self = static_cast<IRReceiver*>(arg);
      self->ISR();
>>>>>>> parent of 143d029 (Problem in ISR attaching inside the class.)
    }

    void ISR() {
      unsigned long time = micros();
      unsigned long duration = time - lastTime;
      lastTime = time;

<<<<<<< HEAD
    if (duration >= (NEC_BIT_MARK - threshold) && duration <= (NEC_BIT_MARK + threshold))
    {
      // Detected bit mark
      bool pinState = invert ? !(GPIO_REG_READ(GPIO_IN_ADDRESS) & (1 << recvPin)) : (GPIO_REG_READ(GPIO_IN_ADDRESS) & (1 << recvPin));
      
      if (pinState)
      {
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
          if (onReceiveHandler && onReceiveEnable)
          {
            onReceiveHandler(data);
          }
=======
      if (duration >= (NEC_HDR_MARK - threshold) && duration <= (NEC_HDR_MARK + threshold)) {
        // Detected header mark
        bitCount = 0;
        data = 0;
        return;
      }

      if (bitCount >= NEC_BITS) {
        // Already received full data
        return;
      }

      if (duration >= (NEC_BIT_MARK - threshold) && duration <= (NEC_BIT_MARK + threshold)) {
        // Detected bit mark
        if (invert) {
          duration = digitalRead(recvPin) ? duration : 0;
        }
        if (duration >= (NEC_ONE_SPACE - threshold) && duration <= (NEC_ONE_SPACE + threshold)) {
          data = (data << 1) | 1;
        } else if (duration >= (NEC_ZERO_SPACE - threshold) && duration <= (NEC_ZERO_SPACE + threshold)) {
          data = (data << 1);
        }
        bitCount++;

        if (bitCount == NEC_BITS) {
          dataAvailable = true;
          OnReceive_execute();
>>>>>>> parent of 143d029 (Problem in ISR attaching inside the class.)
        }
      }
    }

<<<<<<< HEAD
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
=======
  public:
    IRReceiver(int pin, int threshold = 200, bool invertSignal = false)
      : recvPin(pin), threshold(threshold), invert(invertSignal), lastTime(0), data(0), bitCount(0), dataAvailable(false), onReceiveHandler(nullptr) {
      pinMode(recvPin, INPUT);
      if (invert) {
        attachInterruptArg(digitalPinToInterrupt(recvPin), ISRWrapper, this, FALLING);
      } else {
        attachInterruptArg(digitalPinToInterrupt(recvPin), ISRWrapper, this, RISING);
      }
    }

    bool available() {
      return dataAvailable;
    }

    unsigned long read() {
      dataAvailable = false;
      return data;
    }

    void OnReceive_enable(bool enable) {
      if (enable) {
        if (invert) {
          attachInterruptArg(digitalPinToInterrupt(recvPin), ISRWrapper, this, FALLING);
        } else {
          attachInterruptArg(digitalPinToInterrupt(recvPin), ISRWrapper, this, RISING);
        }
      } else {
        detachInterrupt(digitalPinToInterrupt(recvPin));
      }
    }

    void OnReceive_setHandler(OnReceiveHandler handler) {
      onReceiveHandler = handler;
    }

    void OnReceive_execute() {
      if (dataAvailable && onReceiveHandler && enable) {
        onReceiveHandler(data);
      }
    }
>>>>>>> parent of 143d029 (Problem in ISR attaching inside the class.)
};

#endif // IRRECEIVER_HPP
