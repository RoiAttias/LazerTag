/**
 * @file Constants_Gun.h
 * @brief Hardware configuration constants for the Gun device.
 *
 * Defines the GPIO pins and timing parameters for:
 *  - Trigger pushbutton
 *  - IR transmitter (NEC protocol)
 *  - LED strip used for feedback animations
 */

#ifndef CONSTANTS_GUN_H
#define CONSTANTS_GUN_H 

#include <Arduino.h>

//------------------------------------------------------------------------------
// Pushbutton Trigger
//------------------------------------------------------------------------------
/// GPIO pin connected to the gun’s trigger pushbutton (with pull-up internally).
static const uint8_t triggerPin = 27;

//------------------------------------------------------------------------------
// IR Transmitter (NEC Protocol)
//------------------------------------------------------------------------------
/// GPIO pin driving the IR LED emitter.
static const uint8_t irPin = 4;
/// LEDC channel used to generate the IR carrier PWM.
static const uint8_t irChannel = 5;
/// Carrier frequency for NEC protocol (in Hz, typically 38 kHz).
static const uint32_t irFrequency = 38000;

//------------------------------------------------------------------------------
// On-Board LED Strip (NeoPixel) for Visual Feedback
//------------------------------------------------------------------------------
/// GPIO pin connected to the NeoPixel data input.
static const uint8_t stripPin = 26;
/// Number of LEDs on the strip.
static const uint8_t stripLength = 10;
/// Minimum frame interval (in ms) between consecutive strip updates.
static const uint8_t stripFrameIntervalMS = 30;
/// Default brightness level for the strip (0–255).
static const uint8_t stripBrightness = 20;

#endif // CONSTANTS_GUN_H