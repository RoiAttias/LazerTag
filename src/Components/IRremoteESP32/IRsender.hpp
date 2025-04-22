/**
 * @file IRsender.hpp
 * @brief IR transmitter class for sending NEC protocol frames via ESP32 LEDC.
 *
 * Encapsulates pulse generation (mark/space) and provides methods to send
 * NEC commands by bit-banging the carrier on a chosen LEDC channel.
 */

 #ifndef IRSENDER_HPP
 #define IRSENDER_HPP
 
 #include "IRremoteESP32.hpp"
 
 /**
  * @class IRsender
  * @brief Sends NEC IR codes using ESP32 LEDC peripheral.
  */
 class IRsender {
 public:
   /** @brief GPIO pin used for the IR LED output. */
   int ledPin;
   /** @brief LEDC channel (0-15) for PWM generation. */
   int channel;
   /** @brief PWM frequency (e.g., 38000 for 38kHz carrier). */
   int freq;
   /** @brief Invert PWM logic if true. */
   bool invert;
 
   /**
    * @brief Constructs an IRsender instance.
    *
    * @param pin         GPIO pin for the IR LED.
    * @param channel     LEDC channel (0-15).
    * @param frequency   PWM frequency in Hz.
    * @param invertSignal True to invert PWM output.
    */
   IRsender(int pin, int channel, int frequency, bool invertSignal = false)
     : ledPin(pin), channel(channel), freq(frequency), invert(invertSignal) {}
 
   /**
    * @brief Initializes the LEDC peripheral and sets output pin mode.
    */
   void init() {
     pinMode(ledPin, OUTPUT);
     ledcSetup(channel, freq, 10);    // 10-bit resolution
     ledcAttachPin(ledPin, channel);
     space(0);                        // Ensure output is idle
   }
 
   /**
    * @brief Sends an NEC frame (header + 32 bits).
    *
    * @param data Raw 32-bit NEC frame.
    * @param nbits Number of bits to send (default 32).
    */
   void sendNEC(uint32_t data, uint8_t nbits = NEC_BITS) {
     // Header
     mark(NEC_HEADER_MARK);
     space(NEC_HEADER_SPACE);
     // Data bits
     for (uint32_t mask = 1UL << (nbits - 1); mask; mask >>= 1) {
       mark(NEC_BIT_MARK);
       if (data & mask) {
         space(NEC_ONE_SPACE);
       } else {
         space(NEC_ZERO_SPACE);
       }
     }
     // Final bit mark and space
     mark(NEC_BIT_MARK);
     space(0);
   }
 
   /**
    * @brief Sends an NEC frame from a NEC_DATA struct.
    * @param data NEC_DATA union.
    */
   void sendNEC(NEC_DATA data) { sendNEC(data.data); }
 
   /**
    * @brief Sends an NEC frame given address and command bytes.
    *
    * Inverts bytes automatically to match NEC format.
    * @param address Address byte.
    * @param command Command byte.
    */
   void sendNEC(uint8_t address, uint8_t command) {
     NEC_DATA d(address, command);
     sendNEC(d);
   }
 
 protected:
   /**
    * @brief Generates a carrier (mark) for the specified time.
    * @param time Microseconds to enable PWM output.
    */
   void mark(int time) {
     ledcWrite(channel, 512); // 50% duty
     delayMicroseconds(time);
   }
 
   /**
    * @brief Generates a space (no carrier) for the specified time.
    * @param time Microseconds to disable or invert PWM output.
    */
   void space(int time) {
     if (invert) {
       ledcWrite(channel, 1023); // Inverted logic high
     } else {
       ledcWrite(channel, 0);    // PWM off
     }
     delayMicroseconds(time);
   }
 };
 
 #endif // IRSENDER_HPP 