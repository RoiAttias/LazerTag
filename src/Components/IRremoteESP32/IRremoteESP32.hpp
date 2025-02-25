#ifndef IRREMOTEESP32_HPP
#define IRREMOTEESP32_HPP

#include <Arduino.h>

#include "Components/Pushbutton/PushButton.hpp"

#define NEC_BITS 32
#define NEC_HEADER_MARK 9000UL
#define NEC_HEADER_SPACE 4500UL
#define NEC_BIT_MARK 562UL
#define NEC_ONE_SPACE 1687UL
#define NEC_ZERO_SPACE 562UL

#define NEC_END_MARK 562UL

#define NEC_THRESHOLD 100UL
#define NEC_BOUNCE_STOP_FILTER (NEC_THRESHOLD * 2UL)

#define IR_RECEIVER_BUFFER_SIZE 32  // Define buffer size for storing received data

enum NEC_STAGE
{
  headerMark,
  headerSpace,
  bitMark,
  bitSpace,
};

union NEC_DATA
{
  uint32_t data;
  struct
  {
    uint8_t address;
    uint8_t address_inv;
    uint8_t command;
    uint8_t command_inv;
  };

  NEC_DATA(uint32_t value = 0) : data(value) {}
  NEC_DATA(uint8_t addr, uint8_t cmd) : address(addr), address_inv(~addr), command(cmd), command_inv(~cmd) {}

  bool operator==(const NEC_DATA &other) const
  {
    return data == other.data;
  }

  bool operator!=(const NEC_DATA &other) const
  {
    return data != other.data;
  }
};

bool compare (uint32_t value, uint32_t valToCompare, uint32_t epsilon)
{
  return value >= (valToCompare - epsilon) && value <= (valToCompare + epsilon);
}

#include "IRsender.hpp"
#include "IRreceiver.hpp"

#endif // IRREMOTEESP32_HPP