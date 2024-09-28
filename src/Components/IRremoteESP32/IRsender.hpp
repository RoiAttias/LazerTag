#ifndef IRSENDER_HPP
#define IRSENDER_HPP

#include "IRremoteESP32.hpp"

class IRsender {
  private:
    int ledPin;
    int channel;
    int freq;
    bool invert;

    void mark(int time) {
      ledcWrite(channel, 512); // 50% duty cycle
      delayMicroseconds(time);
    }

    void space(int time) {
      if (invert) {
        ledcWrite(channel, 1023); // Inverted logic: Turn on LED
      } else {
        ledcWrite(channel, 0); // Turn off LED
      }
      delayMicroseconds(time);
    }
    
  public:
    IRsender(int pin, int channel, int frequency, bool invertSignal = false)
      : ledPin(pin), channel(channel), freq(frequency), invert(invertSignal) {
      pinMode(ledPin,OUTPUT);
      ledcSetup(channel, freq, 10); // Setup LEDC with 38Khz and 10-bit resolution
      ledcAttachPin(ledPin, channel);
      space(0);
    }

    void sendNEC(unsigned long data, int nbits = 32) {
      mark(NEC_HEADER_MARK);
      space(NEC_HEADER_SPACE);
      for (unsigned long mask = 1UL << (nbits - 1); mask; mask = mask >> 1) {
        if (data & mask) {
          mark(NEC_BIT_MARK);
          space(NEC_ONE_SPACE);
        } else {
          mark(NEC_BIT_MARK);
          space(NEC_ZERO_SPACE);
        }
      }
      mark(NEC_BIT_MARK);
      space(0);
    }
};

#endif // IRSENDER_HPP