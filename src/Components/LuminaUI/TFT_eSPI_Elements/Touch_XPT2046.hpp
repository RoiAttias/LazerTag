/**
 * @file Touch_XPT2046.hpp
 * @brief Driver for XPT2046 touch controller, extending LuminaUI Touch logic.
 *
 * Implements raw touch detection, coordinate sampling and averaging,
 * noise filtering, and screen mapping for a TFT_eSPI-based display.
 */

 #ifndef TOUCH_XPT2046_HPP
 #define TOUCH_XPT2046_HPP
 
 #include "TFT_eSPI_Elements.hpp"   ///< TFT and sprite instances
 #include "Components/LuminaUI/Touch.hpp" ///< Base Touch event processor
 
 /**
  * @class Touch_XPT2046
  * @brief Touch processor for XPT2046 controller and TFT_eSPI.
  *
  * Inherits from LuminaUI::Touch to handle press, release, hold, and drag.
  * Performs pressure-based detection, multiple-sample averaging, noise rejection,
  * and raw-to-screen coordinate mapping including rotation adjustments.
  */
 class Touch_XPT2046 : public Touch {
 private:
     bool wasTouched;                  ///< Previous touch state flag
     uint16_t pressureThreshold = 150; ///< Minimum pressure to register touch
     int pointSamples = 3;             ///< Samples to average for position
     int pressureSamples = 5;          ///< Samples to average for pressure check
 
 public:
     /**
      * @brief Constructor linking Touch to a Screen instance.
      * @param screen Pointer to the associated Screen.
      */
     Touch_XPT2046(Screen* screen)
       : Touch(screen), wasTouched(false) {}
 
     /**
      * @brief Check if the touch controller is pressed.
      *
      * Samples raw Z values and compares against threshold. Majority vote if multiple samples.
      * @param pressureThreshold Minimum raw Z value to consider "touched".
      * @param samplesCount Number of samples to average (default 1).
      * @return True if screen is considered touched.
      */
     virtual bool isTouched(uint16_t pressureThreshold, int samplesCount = 1) {
         if (samplesCount > 1) {
             int valid = 0;
             for (int i = 0; i < samplesCount; ++i) {
                 if (LuminaUI::tft_instance->getTouchRawZ() > pressureThreshold) {
                     ++valid;
                 }
             }
             return valid >= (samplesCount / 2);
         }
         return LuminaUI::tft_instance->getTouchRawZ() > pressureThreshold;
     }
 
     /**
      * @brief Read and average raw touch coordinates, map to screen space.
      *
      * Performs multiple sampling, noise thresholding, linear mapping,
      * and rotation-based coordinate transform.
      * @param sampleCount Number of valid position samples to average (default 1).
      * @return Mapped touch point in screen coordinates.
      */
     virtual ivec2 getPoint(int sampleCount = 1) {
         uint16_t raw[2];
         ivec2 accum(0,0), prev(0,0), curr(0,0);
         int valid = 0;
         const int maxAttempts = sampleCount * 2;
         const int noiseThresh = 50;
 
         for (int i = 0; i < maxAttempts && valid < sampleCount; ++i) {
             LuminaUI::tft_instance->getTouchRaw(&raw[0],&raw[1]);
             curr = ivec2(raw[0], raw[1]);
             if (!valid) prev = curr;
             ivec2 delta = curr - prev;
             if (sqrt(delta.x*delta.x + delta.y*delta.y) > noiseThresh) continue;
             accum += curr;
             prev = curr;
             ++valid;
         }
         if (!valid) return lastPoint;
         ivec2 avg = accum / valid;
 
         // Map raw to screen
         const ivec2 rawMin(140,245), rawMax(4000,4000);
         const ivec2 sz(320,480);
         ivec2 m;
         m.x = constrain(map(avg.x, rawMin.x, rawMax.x, 0, sz.x), 0, sz.x-1);
         m.y = constrain(sz.y - map(avg.y, rawMin.y, rawMax.y, 0, sz.y), 0, sz.y-1);
 
         // Adjust for rotation
         ivec2 result;
         switch (LuminaUI::tft_instance->getRotation()) {
             case 1: result = ivec2(m.y, sz.x-1 - m.x); break;
             case 2: result = ivec2(sz.x-1 - m.x, sz.y-1 - m.y); break;
             case 3: result = ivec2(sz.y-1 - m.y, m.x); break;
             default: result = m;
         }
         lastPoint = result;
         return result;
     }
 
     /**
      * @brief Process raw touch each loop iteration and dispatch events.
      *
      * Call in main loop to detect press, release, and continuous touch.
      */
     void loop() {
         if (!enable) return;
         bool nowTouch = isTouched(pressureThreshold, pressureSamples);
         if (nowTouch && !wasTouched) {
             next(getPoint(pointSamples), true, true);  // Press edge
         } else if (!nowTouch && wasTouched) {
             next(lastPoint, true, false);            // Release edge
         } else if (nowTouch && wasTouched) {
             next(getPoint(pointSamples), false, true);// Continuous touch
         }
         if (nowTouch || wasTouched) wasTouched = nowTouch;
     }
 };
 
 #endif // TOUCH_XPT2046_HPP 