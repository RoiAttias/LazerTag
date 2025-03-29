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
    return (value < minVal) ? minVal : (value > maxVal ? maxVal : value);
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



#endif // MOREMATH_HPP
