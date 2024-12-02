#ifndef IRREMOTEESP32_HPP
#define IRREMOTEESP32_HPP

#include <Arduino.h>

#define NEC_BITS 32
#define NEC_HEADER_MARK 9000UL
#define NEC_HEADER_SPACE 4500UL
#define NEC_BIT_MARK 562UL
#define NEC_ONE_SPACE 1687UL
#define NEC_ZERO_SPACE 562UL
#define NEC_THRESHOLD 100UL
#define NEC_BOUNCE_STOP_FILTER (NEC_THRESHOLD * 2UL)
#define NEC_MAX_LENGTH

#include "IRsender.hpp"
#include "IRreceiver.hpp"

#endif // IRREMOTEESP32_HPP