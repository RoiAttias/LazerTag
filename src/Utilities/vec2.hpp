/**
 * @file vec2.hpp
 * @brief Defines the vec2 struct for 2D floating-point vector operations.
 *
 * Provides a simple, efficient representation and manipulation of 2D vectors
 * in floating-point space, suitable for Arduino projects. Includes common vector
 * operations such as addition, subtraction, scaling, normalization, rotation,
 * dot and cross products, and utility functions for string conversion and serial display.
 */

 #ifndef VEC2_HPP
 #define VEC2_HPP
 
 #include <Arduino.h>
 #include "ivec2.hpp"
 
 /**
  * @brief Struct representing a 2D floating-point vector (similar to OpenGL's vec2).
  *
  * Designed for Arduino applications requiring vector math in graphics, physics,
  * and general computations with floating-point precision. Supports arithmetic
  * operations, geometric transformations, and utility functions.
  */
 struct vec2 {
     /** @brief The x-coordinate (first component) of the vector. */
     float x;
     /** @brief The y-coordinate (second component) of the vector. */
     float y;
 
     /**
      * @brief Constructs a vec2 with specified components.
      * @param x The x-coordinate (default 0.0f).
      * @param y The y-coordinate (default 0.0f).
      */
     vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
 
     /**
      * @brief Converts an ivec2 (integer vector) to a vec2 (floating-point).
      * @param i The ivec2 instance to convert.
      */
     vec2(ivec2 i) : x(i.x), y(i.y) {}
 
     /**
      * @brief Adds another vector to this one (in-place).
      * @param other The vector to add.
      * @return Reference to this vector after addition.
      */
     vec2& operator+=(const vec2& other) {
         x += other.x;
         y += other.y;
         return *this;
     }
 
     /**
      * @brief Subtracts another vector from this one (in-place).
      * @param other The vector to subtract.
      * @return Reference to this vector after subtraction.
      */
     vec2& operator-=(const vec2& other) {
         x -= other.x;
         y -= other.y;
         return *this;
     }
 
     /**
      * @brief Scales this vector by a scalar (in-place).
      * @param scalar The factor to multiply by.
      * @return Reference to this vector after scaling.
      */
     vec2& operator*=(float scalar) {
         x *= scalar;
         y *= scalar;
         return *this;
     }
 
     /**
      * @brief Divides this vector by a scalar (in-place).
      * @param scalar The divisor (ignored if zero).
      * @return Reference to this vector after division.
      */
     vec2& operator/=(float scalar) {
         if (scalar != 0.0f) {
             x /= scalar;
             y /= scalar;
         }
         return *this;
     }
 
     /**
      * @brief Equality comparison operator.
      * @param other The vector to compare against.
      * @return True if both components match exactly.
      */
     bool operator==(const vec2& other) const {
         return (x == other.x) && (y == other.y);
     }
 
     /**
      * @brief Inequality comparison operator.
      * @param other The vector to compare against.
      * @return True if any component differs.
      */
     bool operator!=(const vec2& other) const {
         return !(*this == other);
     }
 
     /**
      * @brief Returns the sum of this vector and another.
      * @param other The vector to add.
      * @return New vec2 representing the sum.
      */
     vec2 operator+(const vec2& other) const {
         return vec2(x + other.x, y + other.y);
     }
 
     /**
      * @brief Returns the difference between this vector and another.
      * @param other The vector to subtract.
      * @return New vec2 representing the difference.
      */
     vec2 operator-(const vec2& other) const {
         return vec2(x - other.x, y - other.y);
     }
 
     /**
      * @brief Scales this vector by a scalar.
      * @param scalar The factor to multiply by.
      * @return New vec2 after scaling.
      */
     vec2 operator*(float scalar) const {
         return vec2(x * scalar, y * scalar);
     }
 
     /**
      * @brief Divides this vector by a scalar.
      * @param scalar The divisor.
      * @return New vec2 after division, or (0,0) if scalar is zero.
      */
     vec2 operator/(float scalar) const {
         if (scalar != 0.0f) {
             return vec2(x / scalar, y / scalar);
         }
         return vec2(0.0f, 0.0f);
     }
 
     /**
      * @brief Returns the negation of this vector.
      * @return New vec2 with both components negated.
      */
     vec2 operator-() const {
         return vec2(-x, -y);
     }
 
     /**
      * @brief Component-wise multiplication.
      * @param other The vector to multiply by.
      * @return New vec2 with multiplied components.
      */
     vec2 multiply(const vec2& other) const {
         return vec2(x * other.x, y * other.y);
     }
 
     /**
      * @brief Calculates the angle of the vector in radians.
      * @return Angle measured from the positive x-axis.
      */
     float angle() const {
         return atan2(y, x);
     }
 
     /**
      * @brief Computes the dot product with another vector.
      * @param other The vector to dot with.
      * @return Scalar dot product.
      */
     float dot(const vec2& other) const {
         return (x * other.x) + (y * other.y);
     }
 
     /**
      * @brief Computes the 2D cross product magnitude.
      * @param other The other vector.
      * @return z-component of the 3D cross product (scalar value).
      */
     float cross(const vec2& other) const {
         return x * other.y - y * other.x;
     }
 
     /**
      * @brief Computes the distance to another vector.
      * @param other The target vector.
      * @return Euclidean distance between the two points.
      */
     float distanceTo(const vec2& other) const {
         float dx = x - other.x;
         float dy = y - other.y;
         return sqrt(dx * dx + dy * dy);
     }
 
     /**
      * @brief Computes the magnitude (length) of the vector.
      * @return Length of the vector.
      */
     float magnitude() const {
         return sqrt(x * x + y * y);
     }
 
     /**
      * @brief Returns a normalized (unit-length) version of the vector.
      * @return New vec2 of length 1, or (0,0) if original is zero-length.
      */
     vec2 normalize() const {
         float mag = magnitude();
         if (mag > 0.0f) {
             return vec2(x / mag, y / mag);
         }
         return vec2(0.0f, 0.0f);
     }
 
     /**
      * @brief Rotates the vector by a given angle.
      * @param angle_rad Angle in radians.
      * @return New vec2 representing the rotated vector.
      */
     vec2 rotate(float angle_rad) const {
         float c = cos(angle_rad);
         float s = sin(angle_rad);
         return vec2(x * c - y * s, x * s + y * c);
     }
 
     /**
      * @brief Converts the vector to a human-readable string.
      * @return Arduino String in the format "(x, y)".
      */
     String toString() const {
         return String("(") + x + ", " + y + ")";
     }
 
     /**
      * @brief Prints the vector to the Serial Monitor.
      *
      * Note: Uses Arduino Serial, so ensure Serial.begin() has been called.
      *
      * @param newLine If true, append newline after printing.
      */
     void display(bool newLine = true) const {
         if (newLine) {
             Serial.println(this->toString());
         } else {
             Serial.print(this->toString());
         }
     }
 };
 
 #endif // VEC2_HPP 