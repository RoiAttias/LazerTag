#ifndef IRSENDER_HPP
#define IRSENDER_HPP

#include <Arduino.h>

class IRsender {
  private:
    int ledPin;
    int channel;
    int freq;
    bool invert;
    
  public:
    IRsender(int pin, int channel, int frequency, bool invertSignal = false)
      : ledPin(pin), channel(channel), freq(frequency), invert(invertSignal) {
      ledcSetup(channel, freq, 10); // Setup LEDC with 10-bit resolution
      ledcAttachPin(ledPin, channel);
      space(0);
    }

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

    void sendNEC(unsigned long data, int nbits = 32) {
      mark(9000);
      space(4500);
      for (unsigned long mask = 1UL << (nbits - 1); mask; mask >> 1) {
        if (data & mask) {
          mark(560);
          space(1690);
        } else {
          mark(560);
          space(560);
        }
      }
      mark(560);
      space(0);
    }
};

#endif // IRSENDER_HPP