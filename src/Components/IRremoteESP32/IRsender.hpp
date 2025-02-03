#ifndef IRSENDER_HPP
#define IRSENDER_HPP

#include "IRremoteESP32.hpp"

class IRsender {
  public:
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
      : ledPin(pin), channel(channel), freq(frequency), invert(invertSignal) {}
    
    
    void init() {
      pinMode(ledPin,OUTPUT);
      ledcSetup(channel, freq, 10); // Setup LEDC with 38Khz and 10-bit resolution
      ledcAttachPin(ledPin, channel);
      space(0);
    }

    

    void sendNEC(uint32_t data, uint8_t nbits = 32) {
      mark(NEC_HEADER_MARK);
      space(NEC_HEADER_SPACE);
      for (uint32_t mask = 1UL << (nbits - 1); mask; mask = mask >> 1) {
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

    void sendNEC(NEC_DATA data) {
      sendNEC(data.data);
    }

    void sendNEC(uint8_t address, uint8_t command) {
      NEC_DATA data;
      data.address = address;
      data.address_inv = ~address;
      data.command = command;
      data.command_inv = ~command;
      sendNEC(data);
    }
};

#endif // IRSENDER_HPP