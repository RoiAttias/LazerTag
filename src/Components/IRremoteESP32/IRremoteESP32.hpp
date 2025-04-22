/**
 * @file IRremoteESP32.hpp
 * @brief Core definitions and utilities for ESP32-based NEC IR remote control.
 *
 * Provides NEC protocol timing constants, comparison helper, and data structures
 * for packing and unpacking NEC command frames. Includes sender and receiver headers.
 */

 #ifndef IRREMOTEESP32_HPP
 #define IRREMOTEESP32_HPP
 
 #include <Arduino.h>
 #include "Components/Pushbutton/PushButton.hpp"
 
 /** Number of bits in NEC protocol frames. */
 #define NEC_BITS 32
 
 /** NEC protocol header mark duration (microseconds). */
 #define NEC_HEADER_MARK 9000UL
 /** NEC protocol header space duration (microseconds). */
 #define NEC_HEADER_SPACE 4500UL
 /** NEC protocol bit mark duration (microseconds). */
 #define NEC_BIT_MARK 562UL
 /** NEC protocol "1" space duration (microseconds). */
 #define NEC_ONE_SPACE 1687UL
 /** NEC protocol "0" space duration (microseconds). */
 #define NEC_ZERO_SPACE 562UL
 
 /** Timing tolerance for pulse comparison (microseconds). */
 #define NEC_THRESHOLD 100UL
 /** Minimum gap between signals to avoid bounce (microseconds). */
 #define NEC_BOUNCE_STOP_FILTER (NEC_THRESHOLD * 2UL)
 
 /** Time window (milliseconds) within which repeats are considered valid. */
 #define NEC_VALID_TIME_MS 70
 
 /** Size of the circular buffer for received IR data frames. */
 #define IR_RECEIVER_BUFFER_SIZE 32
 
 /**
  * @brief Compare a measured duration to an expected value within a tolerance.
  *
  * @param value Measured duration.
  * @param valToCompare Expected duration.
  * @param epsilon Maximum allowed deviation.
  * @return True if |value - valToCompare| <= epsilon.
  */
 inline bool compare(uint32_t value, uint32_t valToCompare, uint32_t epsilon) {
   return (value >= (valToCompare - epsilon)) && (value <= (valToCompare + epsilon));
 }
 
 /**
  * @brief Stages of the NEC receive state machine.
  */
 enum NEC_STAGE {
   headerMark,  /**< Waiting for the header mark pulse. */
   headerSpace, /**< Waiting for the header space. */
   bitMark,     /**< Waiting for a bit mark. */
   bitSpace,    /**< Waiting for the bit space (0 or 1). */
 };
 
 /**
  * @brief Union representing an NEC frame: address and command with inverses.
  */
 union NEC_DATA {
   uint32_t data;      /**< Raw 32-bit value of the frame. */
   struct {
     uint8_t address;      /**< Address byte. */
     uint8_t address_inv;  /**< Bitwise inverse of address. */
     uint8_t command;      /**< Command byte. */
     uint8_t command_inv;  /**< Bitwise inverse of command. */
   };
 
   /** Default constructor initializes data to zero. */
   NEC_DATA(uint32_t value = 0) : data(value) {}
   /** Construct from address and command (inverts automatically). */
   NEC_DATA(uint8_t addr, uint8_t cmd)
     : address(addr), address_inv(~addr), command(cmd), command_inv(~cmd) {}
 
   /** Equality operator compares full 32-bit data. */
   bool operator==(const NEC_DATA &other) const { return data == other.data; }
   /** Inequality operator. */
   bool operator!=(const NEC_DATA &other) const { return data != other.data; }
 };
 
 #include "IRsender.hpp"
 #include "IRreceiver.hpp"
 
 #endif // IRREMOTEESP32_HPP