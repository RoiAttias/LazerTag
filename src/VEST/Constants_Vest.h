/**
 * @file Constants_Vest.h
 * @brief Hardware configuration constants for the Vest device.
 *
 * Defines the IR receiver pins and NeoPixel strip parameters.
 */

 #ifndef CONSTANTS_VEST_H
 #define CONSTANTS_VEST_H
 
 //------------------------------------------------------------------------------
 // IR Receiver Configuration
 //------------------------------------------------------------------------------
 /// GPIO pins for the three IR receivers
 static const uint8_t recvPins[] = {27, 26, 25};
 /// If true, receiver will validate inverted address/command bytes
 static const bool recvValid = false;
 
 //------------------------------------------------------------------------------
 // NeoPixel Ring Configuration
 //------------------------------------------------------------------------------
 /// GPIO pin connected to the NeoPixel data line
 static const uint8_t stripPin = 14;
 /// Number of LEDs on the NeoPixel ring
 static const uint8_t stripLength = 24;
 /// Milliseconds between each animation frame
 static const uint8_t stripFrameIntervalMS = 30;
 /// Default global brightness (0–255)
 static const uint8_t stripBrightness = 100;
 
 #endif // CONSTANTS_VEST_H 