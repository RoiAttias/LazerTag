#ifndef MOREMATH_HPP
#define MOREMATH_HPP

#include <Arduino.h>
#include "vec2.hpp"
#include "ivec2.hpp"

// Precomputed constants for conversions
constexpr float DEG_TO_RAD_FACTOR = PI / 180.0f; ///< Conversion factor from degrees to radians.
constexpr float RAD_TO_DEG_FACTOR = 180.0f / PI; ///< Conversion factor from radians to degrees.

/**
 * @brief Converts degrees to radians.
 * 
 * @param deg Angle in degrees.
 * @return Angle in radians.
 */
inline float degToRad(float deg)
{
    return deg * DEG_TO_RAD_FACTOR;
}

/**
 * @brief Converts radians to degrees.
 * 
 * @param rad Angle in radians.
 * @return Angle in degrees.
 */
inline float radToDeg(float rad)
{
    return rad * RAD_TO_DEG_FACTOR;
}

/**
 * @brief Clamps a value within a specified range.
 * 
 * @param value The input value to clamp.
 * @param minVal The lower bound of the range.
 * @param maxVal The upper bound of the range.
 * @return The clamped value.
 */
inline float clamp(float value, float minVal, float maxVal)
{
    if (value < minVal)
        return minVal;
    else if (value > maxVal)
        return maxVal;
    else
        return value;
    //return (value < minVal) ? minVal : (value > maxVal ? maxVal : value);
}

/**
 * @brief Performs linear interpolation between two values.
 * 
 * @param start The starting value.
 * @param end The ending value.
 * @param t A float between 0 and 1 representing the interpolation factor.
 * @return The interpolated value.
 */
inline float lerp(float start, float end, float t)
{
    return start + t * (end - start);
}

/**
 * @brief Mixes two values based on a factor. 
 * @param factor The mixing factor.
 * @param v1 The first value.
 * @param v2 The second value.
 * @return The mixed value.
 */
inline float mix(float factor, float v1, float v2)
{
    return (v1 * (1.0f - factor)) + (v2 * factor);
}

/**
 * @brief Mixes two integer values based on a factor. 
 * @param factor The mixing factor.
 * @param v1 The first value.
 * @param v2 The second value.
 * @return The mixed value.
 */
inline int mix(float factor, int v1, int v2)
{
    return (int)(mix(factor, (float)v1, (float)v2));
}

/**
 * @brief Mixes two byte values based on a factor. 
 * @param factor The mixing factor.
 * @param v1 The first value.
 * @param v2 The second value.
 * @return The mixed value.
 */
inline byte mix(float factor, byte v1, byte v2)
{
    return (byte)(mix(factor, (float)v1, (float)v2));
}

/**
 * @brief Maps a value from one range to another range.
 * @param val The value to map.
 * @param fromMin The lower bound of the input range.
 * @param fromMax The upper bound of the input range.
 * @param toMin The lower bound of the output range.
 * @param toMax The upper bound of the output range.
 * @return The mapped value in the output range.
 */
inline int map(int val, int fromMin, int fromMax, int toMin, int toMax)
{
    int distance = fromMax - fromMin;
    val -= fromMin;
    float factor = (float)val / distance;
    return mix(factor, toMin, toMax);
}

/**
 * @brief Maps a value from one range to another range.
 * 
 * @param value The input value to map.
 * @param inMin The lower bound of the input range.
 * @param inMax The upper bound of the input range.
 * @param outMin The lower bound of the output range.
 * @param outMax The upper bound of the output range.
 * @return The mapped value in the output range.
 */
inline float mapFloat(float value, float inMin, float inMax, float outMin, float outMax)
{
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

/**
 * @brief Wraps an angle in degrees to the range [0, 360).
 * 
 * @param angle The input angle in degrees.
 * @return The wrapped angle in degrees.
 */
inline float wrapAngleDeg(float angle)
{
    while (angle >= 360.0f) angle -= 360.0f;
    while (angle < 0.0f) angle += 360.0f;
    return angle;
}

/**
 * @brief Wraps an angle in radians to the range [0, 2π).
 * 
 * @param angle The input angle in radians.
 * @return The wrapped angle in radians.
 */
inline float wrapAngleRad(float angle)
{
    while (angle >= 2.0f * PI) angle -= 2.0f * PI;
    while (angle < 0.0f) angle += 2.0f * PI;
    return angle;
}

/**
 * @brief Computes the distance between two 2d points.
 * 
 * @param x1 The x-coordinate of the first point.
 * @param y1 The y-coordinate of the first point.
 * @param x2 The x-coordinate of the second point.
 * @param y2 The y-coordinate of the second point.
 */
inline float distance(float x1, float y1, float x2, float y2)
{
    return sqrt(sq(x2 - x1) + sq(y2 - y1));
}

/**
 * @brief Calculates colors in RGB format based on a hue value.
 *
 * @param hue The hue value (range [0, 1]).
 * @param red Pointer to store the red component (0-255).
 * @param green Pointer to store the green component (0-255).
 * @param blue Pointer to store the blue component (0-255).
 */
inline void hueToRgb(float hue, byte *red, byte *green, byte *blue)
{
    // This function converts a hue value (range [0, 1]) to an RGB color
    // with full saturation and brightness. The resulting RGB components
    // are scaled to the range [0, 255].

    // Wrap hue to the range [0, 1] in case it exceeds 1.
    float normalizedHue = fmod(hue, 1.0f);

    // Scale hue to [0, 6) to determine its position on the color wheel.
    float hueScaled = normalizedHue * 6.0f;

    // Determine which of the 6 sectors of the color wheel the hue falls into.
    int sectorIndex = (int) floor(hueScaled) % 6;

    // Calculate the fractional part within the sector.
    float fractionWithinSector = hueScaled - sectorIndex;
    // Compute the complementary fraction (used for interpolation).
    float inverseFraction = 1.0f - fractionWithinSector;

    float r, g, b;
    // Determine the RGB values based on the sector.
    switch (sectorIndex)
    {
        // Red -> Yellow: red is at full intensity; green increases with fraction.
        case 0: r = 1.0f; g = fractionWithinSector; b = 0.0f; break;
        // Yellow -> Green: green is at full intensity; red decreases with inverse fraction.
        case 1: r = inverseFraction; g = 1.0f; b = 0.0f; break;
        // Green -> Cyan: green is at full intensity; blue increases with fraction.
        case 2: r = 0.0f; g = 1.0f; b = fractionWithinSector; break;
        // Cyan -> Blue: blue is at full intensity; green decreases with inverse fraction.
        case 3: r = 0.0f; g = inverseFraction; b = 1.0f; break;
        // Blue -> Magenta: blue is at full intensity; red increases with fraction.
        case 4: r = fractionWithinSector; g = 0.0f; b = 1.0f; break;
        // Magenta -> Red: red is at full intensity; blue decreases with inverse fraction.
        case 5: r = 1.0f; g = 0.0f; b = inverseFraction; break;
        default: r = 0.0f; g = 0.0f; b = 0.0f; break; // Fallback (should never occur)
    }

    // Convert the computed RGB values from [0, 1] to [0, 255]
    *red   = (byte)(r * 255.0f);
    *green = (byte)(g * 255.0f);
    *blue  = (byte)(b * 255.0f);
}


#endif // MOREMATH_HPP
