/**
 * @file Ring.hpp
 * @brief LED animation subsystem for the Vest’s NeoPixel ring.
 *
 * Defines color constants, animation callback functions, and Animation objects.
 * Provides a simple namespace API to trigger effects (loading, hits, HP bar, win/lose, etc.).
 */

 #ifndef RING_HPP
 #define RING_HPP
 
 #include <Arduino.h>
 #include "VEST/Constants_Vest.h"       ///< stripPin, stripLength, stripFrameIntervalMS, stripBrightness
 #include "Components/Visualizer/Visualizer.hpp" ///< Animation, Visualizer
 #include "Utilities/MoreMath.hpp"      ///< clamp(), distance(), mix()
 
 // Hue definitions for HSV color wheel
 #define HUE_RED    0                // 0 degrees
 #define HUE_GREEN  (0xFFFF / 3)     // 120 degrees
 #define HUE_BLUE   (HUE_GREEN * 2)  // 240 degrees
 #define HUE_YELLOW (HUE_GREEN / 2)  // 60 degrees
 #define HUE_PURPLE ((HUE_YELLOW / 2) + HUE_BLUE) // 270 degrees
 
 namespace Ring {
     extern int      hp;           ///< Current health percentage (0–100)
     extern uint16_t flashingHue;  ///< Hue for mark/flashing animations
     extern uint16_t load2Hue;     ///< Hue for the second loading animation
 }
 
 //----------------------------
 // Animation callback functions
 //----------------------------
 
 /**
  * @brief Rainbow‐pulsing loading effect.
  */
 void load1AnimationFunc(Adafruit_NeoPixel* strip, uint16_t start, uint16_t len, float factor) {
     int center = mix(factor, start, start + len);
     const float maxD = 3.0f;
     const uint32_t cycle = 3000;
     for (uint16_t i = 0; i < len; ++i) {
         uint16_t hue = map(millis() % cycle, 0, cycle, 0, 0xFFFF);
         float brightness = pow(
           clamp((maxD - distance(i,0,center,0)) / maxD, 0, 1) +
           clamp((maxD - distance(i,0,center+len,0)) / maxD, 0, 1),
           3
         );
         strip->setPixelColor(start + i, strip->ColorHSV(hue, 255, uint8_t(brightness * 255)));
     }
 }
 
 /**
  * @brief Single‐color loading effect.
  */
 void load2AnimationFunc(Adafruit_NeoPixel* strip, uint16_t start, uint16_t len, float factor) {
     int center = mix(factor, start, start + len);
     const float maxD = 3.0f;
     for (uint16_t i = 0; i < len; ++i) {
         float brightness = pow(
           clamp((maxD - distance(i,0,center,0)) / maxD, 0, 1) +
           clamp((maxD - distance(i,0,center+len,0)) / maxD, 0, 1),
           3
         );
         strip->setPixelColor(start + i, strip->ColorHSV(Ring::load2Hue, 255, uint8_t(brightness * 255)));
     }
 }
 
 /**
  * @brief Brief flash on hit, hue based on HP.
  */
 void hitAnimationFunc(Adafruit_NeoPixel* strip, uint16_t start, uint16_t len, float factor) {
     uint16_t hue = mix(Ring::hp / 100.0f, HUE_RED, HUE_GREEN);
     uint8_t bri = uint8_t(clamp(1 - 2 * fabs(distance(factor,0,0.5f,0)), 0, 1) * 255);
     for (uint16_t i = 0; i < len; ++i) {
         strip->setPixelColor(start + i, strip->ColorHSV(hue, 255, bri));
     }
 }
 
 /**
  * @brief Continuous HP bar display.
  */
 void hpAnimationFunc(Adafruit_NeoPixel* strip, uint16_t start, uint16_t len, float factor) {
     uint16_t hue = mix(Ring::hp / 100.0f, HUE_RED, HUE_GREEN);
     int count = int(len * Ring::hp / 100.0f);
     for (int i = 0; i < count; ++i) {
         uint8_t bri = uint8_t(mix(
           clamp(1 - 2 * fabs(distance(factor,0,0.5f,0)), 0, 1),
           200, 255
         ));
         strip->setPixelColor(start + i, strip->ColorHSV(hue, 255, bri));
     }
 }
 
 /**
  * @brief Flash “mark” effect.
  */
 void markedAnimationFunc(Adafruit_NeoPixel* strip, uint16_t start, uint16_t len, float factor) {
     uint8_t bri = uint8_t(clamp(1 - 2 * fabs(distance(factor,0,0.5f,0)), 0, 1) * 255);
     for (uint16_t i = 0; i < len; ++i) {
         strip->setPixelColor(start + i, strip->ColorHSV(Ring::flashingHue, 255, bri));
     }
 }
 
 // Additional animations (win/lose) omitted for brevity…
 
 //----------------------------
 // Animation object instances
 //----------------------------
 static Animation load1Animation   (load1AnimationFunc,   1, 0, stripLength, 2000, true);
 static Animation load2Animation   (load2AnimationFunc,   1, 0, stripLength,  500, true);
 static Animation hitAnimation     (hitAnimationFunc,     2, 0, stripLength,  100, false);
 static Animation hpAnimation      (hpAnimationFunc,      0, 0, stripLength, 1000, true);
 static Animation markedAnimation  (markedAnimationFunc,  2, 0, stripLength, 1000, false);
 
 //----------------------------
 // Ring namespace API
 //----------------------------
 namespace Ring {
     Visualizer visualizer(stripPin, stripLength, stripFrameIntervalMS);
     int hp = 0;
     uint16_t flashingHue;
     uint16_t load2Hue;
 
     /** @brief Initialize the NeoPixel ring. */
     void init() {
         visualizer.init(stripBrightness);
     }
 
     /** @brief Advance all active animations. */
     void loop() {
         visualizer.loop();
     }
 
     /** @brief Start the first loading animation. */
     void load1() {
         visualizer.clearAnimations();
         visualizer.addAnimation(load1Animation);
     }
 
     /** @brief Start the second loading animation (green). */
     void load2() {
         load2Hue = HUE_GREEN;
         visualizer.clearAnimations();
         visualizer.addAnimation(load2Animation);
     }
 
     /** @brief Show a 1–3 step countdown or GO (0). */
     void countdown(int num) {
         if (num < 0 || num > 3) return;
         flashingHue = (num == 0 ? HUE_GREEN : HUE_RED);
         visualizer.clearAnimations();
         for (int i = 0; i < num; ++i) {
             // Adjust start index for each segment
             // (assuming even division of ring into 3 parts)
             visualizer.addAnimation(Animation(
               markedAnimationFunc,
               2,
               i * stripLength / 3,
               stripLength / 3,
               600, false
             ));
         }
     }
 
     /** @brief Flash hit animation and update HP for hue mapping. */
     void hit(int newHP) {
         hp = newHP;
         visualizer.addAnimation(hitAnimation);
     }
 
     /** @brief Display HP bar at the start of the game. */
     void onGameStart(int newHP) {
         hp = newHP;
         visualizer.clearAnimations();
         visualizer.addAnimation(hpAnimation);
     }
 
     /** @brief Brief yellow mark effect. */
     void mark() {
         flashingHue = HUE_YELLOW;
         visualizer.addAnimation(markedAnimation);
     }

     /** @brief Brief purple mark effect. */
     void demark() {
        flashingHue = HUE_PURPLE;
        visualizer.addAnimation(markedAnimation);
    }
 
     /** @brief Show game-over loading effect (red). */
     void over() {
         load2Hue = HUE_RED;
         visualizer.clearAnimations();
         visualizer.addAnimation(load2Animation);
     }
 
     /** @brief Victory animation placeholder. */
     void win() {
        visualizer.clearAnimations();
        visualizer.addAnimation(winAnimation);
     }
 
     /** @brief Defeat animation placeholder. */
     void lose() {
        visualizer.clearAnimations();
        visualizer.addAnimation(loseAnimation);
     }
 }
 
 #endif // RING_HPP 