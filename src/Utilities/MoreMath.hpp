/**
 * @file MoreMath.hpp
 * @brief Provides a collection of common math utility functions and constants for Arduino.
 *
 * Includes conversion between degrees and radians, clamping, interpolation,
 * mapping between ranges, angle wrapping, distance calculations, and hue-to-RGB conversion.
 * Designed for 2D graphics, signal processing, and general numeric operations on microcontrollers.
 */

 #ifndef MOREMATH_HPP
 #define MOREMATH_HPP
 
 #include <Arduino.h>
 #include "vec2.hpp"
 #include "ivec2.hpp"
 
 /** @brief Factor to convert degrees to radians. */
 constexpr float DEG_TO_RAD_FACTOR = PI / 180.0f;
 /** @brief Factor to convert radians to degrees. */
 constexpr float RAD_TO_DEG_FACTOR = 180.0f / PI;
 
 /**
  * @brief Converts an angle from degrees to radians.
  *
  * @param deg Angle in degrees.
  * @return Angle in radians.
  */
 inline float degToRad(float deg) {
     return deg * DEG_TO_RAD_FACTOR;
 }
 
 /**
  * @brief Converts an angle from radians to degrees.
  *
  * @param rad Angle in radians.
  * @return Angle in degrees.
  */
 inline float radToDeg(float rad) {
     return rad * RAD_TO_DEG_FACTOR;
 }
 
 /**
  * @brief Clamps a value between a minimum and maximum.
  *
  * @param value Input value to clamp.
  * @param minVal Lower bound.
  * @param maxVal Upper bound.
  * @return Clamped value within [minVal, maxVal].
  */
 inline float clamp(float value, float minVal, float maxVal) {
     if (value < minVal) return minVal;
     if (value > maxVal) return maxVal;
     return value;
 }
 
 /**
  * @brief Performs linear interpolation between two floats.
  *
  * @param start Starting value.
  * @param end Ending value.
  * @param t  Interpolation factor in [0,1].
  * @return Interpolated value: start + t*(end-start).
  */
 inline float lerp(float start, float end, float t) {
     return start + t * (end - start);
 }
 
 /**
  * @brief Mixes two float values by a factor.
  *
  * Alias for lerp, with arguments order: mix(factor, v1, v2) = lerp(v1,v2,factor).
  *
  * @param factor Mixing factor in [0,1].
  * @param v1 First value.
  * @param v2 Second value.
  * @return Mixed value.
  */
 inline float mix(float factor, float v1, float v2) {
     return v1 * (1.0f - factor) + v2 * factor;
 }
 
 /**
  * @brief Mixes two integer values by a factor.
  *
  * @param factor Mixing factor in [0,1].
  * @param v1 First integer value.
  * @param v2 Second integer value.
  * @return Mixed integer (rounded down).
  */
 inline int mix(float factor, int v1, int v2) {
     return static_cast<int>(mix(factor, static_cast<float>(v1), static_cast<float>(v2)));
 }
 
 /**
  * @brief Mixes two byte values by a factor.
  *
  * @param factor Mixing factor in [0,1].
  * @param v1 First byte value.
  * @param v2 Second byte value.
  * @return Mixed byte (rounded down).
  */
 inline byte mix(float factor, byte v1, byte v2) {
     return static_cast<byte>(mix(factor, static_cast<float>(v1), static_cast<float>(v2)));
 }
 
 /**
  * @brief Maps an integer value from one range to another.
  *
  * @param val     Input value to map.
  * @param fromMin Lower bound of input range.
  * @param fromMax Upper bound of input range.
  * @param toMin   Lower bound of output range.
  * @param toMax   Upper bound of output range.
  * @return Mapped integer value in [toMin, toMax].
  */
 inline int map(int val, int fromMin, int fromMax, int toMin, int toMax) {
     int distance = fromMax - fromMin;
     val -= fromMin;
     float factor = static_cast<float>(val) / static_cast<float>(distance);
     return mix(factor, toMin, toMax);
 }
 
 /**
  * @brief Maps a float value from one range to another.
  *
  * @param value Input value to map.
  * @param inMin Lower bound of input range.
  * @param inMax Upper bound of input range.
  * @param outMin Lower bound of output range.
  * @param outMax Upper bound of output range.
  * @return Mapped float value in [outMin, outMax].
  */
 inline float mapFloat(float value, float inMin, float inMax, float outMin, float outMax) {
     return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
 }
 
 /**
  * @brief Wraps an angle in degrees to the range [0,360).
  *
  * @param angleDeg Input angle in degrees.
  * @return Equivalent angle in [0,360).
  */
 inline float wrapAngleDeg(float angleDeg) {
     while (angleDeg >= 360.0f) angleDeg -= 360.0f;
     while (angleDeg <    0.0f) angleDeg += 360.0f;
     return angleDeg;
 }
 
 /**
  * @brief Wraps an angle in radians to the range [0,2π).
  *
  * @param angleRad Input angle in radians.
  * @return Equivalent angle in [0,2π).
  */
 inline float wrapAngleRad(float angleRad) {
     const float twoPi = 2.0f * PI;
     while (angleRad >= twoPi) angleRad -= twoPi;
     while (angleRad <       0.0f) angleRad += twoPi;
     return angleRad;
 }
 
 /**
  * @brief Computes Euclidean distance between two 2D points.
  *
  * @param x1 X-coordinate of first point.
  * @param y1 Y-coordinate of first point.
  * @param x2 X-coordinate of second point.
  * @param y2 Y-coordinate of second point.
  * @return Euclidean distance between the points.
  */
 inline float distance(float x1, float y1, float x2, float y2) {
     return sqrt(sq(x2 - x1) + sq(y2 - y1));
 }
 
 /**
  * @brief Converts a normalized hue value to RGB components.
  *
  * Uses full saturation and brightness. Hue wraps in [0,1]
  * across the color wheel, mapping to RGB.
  *
  * @param hue   Normalized hue in [0,1].
  * @param red   Pointer to output red component (0-255).
  * @param green Pointer to output green component (0-255).
  * @param blue  Pointer to output blue component (0-255).
  */
 inline void hueToRgb(float hue, byte* red, byte* green, byte* blue) {
     float h = fmod(hue, 1.0f);
     float hs = h * 6.0f;
     int sector = static_cast<int>(floor(hs)) % 6;
     float frac = hs - sector;
     float inv = 1.0f - frac;
     float r, g, b;
     switch (sector) {
         case 0: r = 1; g = frac; b = 0; break;
         case 1: r = inv; g = 1; b = 0; break;
         case 2: r = 0; g = 1; b = frac; break;
         case 3: r = 0; g = inv; b = 1; break;
         case 4: r = frac; g = 0; b = 1; break;
         case 5: default: r = 1; g = 0; b = inv; break;
     }
     *red   = static_cast<byte>(r * 255.0f);
     *green = static_cast<byte>(g * 255.0f);
     *blue  = static_cast<byte>(b * 255.0f);
 }
 
 #endif // MOREMATH_HPP 