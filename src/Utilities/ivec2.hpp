/**
 * @file ivec2.hpp
 * @brief Defines the ivec2 class for 2D integer vector operations.
 *
 * Provides a simple, efficient representation and manipulation of 2D vectors
 * in integer space, suitable for Arduino projects. Includes common vector
 * operations such as addition, subtraction, scaling, clamping, and comparison,
 * along with utility functions for string conversion and serial display.
 */

 #ifndef IVEC2_HPP
 #define IVEC2_HPP
 
 #include <Arduino.h>
 
 /**
  * @brief Struct representing a point (or vector) in 2D space with integer coordinates.
  *
  * Designed for Arduino projects, enabling vector math in graphics, physics,
  * and general calculations with integer precision. Supports arithmetic
  * operations, component-wise comparisons, and utility functions.
  */
 class ivec2 {
 public:
     int x;  ///< X-coordinate or first component of the vector
     int y;  ///< Y-coordinate or second component of the vector
 
     /**
      * @brief Constructs a 2D integer vector with given coordinates.
      *
      * @param x The x-coordinate (default 0).
      * @param y The y-coordinate (default 0).
      */
     ivec2(int x = 0, int y = 0) : x(x), y(y) {}
 
     /**
      * @brief Addition assignment operator (+=).
      *
      * @param other The vector to add.
      * @return Reference to this vector after addition.
      */
     ivec2& operator+=(const ivec2& other) {
         x += other.x;
         y += other.y;
         return *this;
     }
 
     /**
      * @brief Subtraction assignment operator (-=).
      *
      * @param other The vector to subtract.
      * @return Reference to this vector after subtraction.
      */
     ivec2& operator-=(const ivec2& other) {
         x -= other.x;
         y -= other.y;
         return *this;
     }
 
     /**
      * @brief Scalar multiplication assignment operator (*=).
      *
      * @param scalar The integer to multiply both components by.
      * @return Reference to this vector after scaling.
      */
     ivec2& operator*=(int scalar) {
         x *= scalar;
         y *= scalar;
         return *this;
     }
 
     /**
      * @brief Scalar division assignment operator (/=).
      *
      * @param scalar The integer to divide both components by.
      * @return Reference to this vector after division (no effect if scalar is zero).
      */
     ivec2& operator/=(int scalar) {
         if (scalar != 0) {
             x /= scalar;
             y /= scalar;
         }
         return *this;
     }
 
     /**
      * @brief Equality comparison operator (==).
      *
      * @param other The vector to compare against.
      * @return True if both x and y are equal.
      */
     bool operator==(const ivec2& other) const {
         return (x == other.x) && (y == other.y);
     }
 
     /**
      * @brief Inequality comparison operator (!=).
      *
      * @param other The vector to compare against.
      * @return True if any component differs.
      */
     bool operator!=(const ivec2& other) const {
         return !(*this == other);
     }
 
     /**
      * @brief Vector addition operator (+).
      *
      * @param other The vector to add.
      * @return New ivec2 representing the sum.
      */
     ivec2 operator+(const ivec2& other) const {
         return ivec2(x + other.x, y + other.y);
     }
 
     /**
      * @brief Vector subtraction operator (-).
      *
      * @param other The vector to subtract.
      * @return New ivec2 representing the difference.
      */
     ivec2 operator-(const ivec2& other) const {
         return ivec2(x - other.x, y - other.y);
     }
 
     /**
      * @brief Scalar multiplication operator (*).
      *
      * @param scalar The integer to multiply by.
      * @return New ivec2 after scaling.
      */
     ivec2 operator*(int scalar) const {
         return ivec2(x * scalar, y * scalar);
     }
 
     /**
      * @brief Scalar division operator (/).
      *
      * @param scalar The integer to divide by.
      * @return New ivec2 after division, or (0,0) if scalar is zero.
      */
     ivec2 operator/(int scalar) const {
         if (scalar != 0) {
             return ivec2(x / scalar, y / scalar);
         }
         return ivec2(0, 0);
     }
 
     /**
      * @brief Unary negation operator (-).
      *
      * @return New ivec2 where both components are negated.
      */
     ivec2 operator-() const {
         return ivec2(-x, -y);
     }
 
     /**
      * @brief Component-wise less-than comparison (<).
      *
      * @param other The vector to compare.
      * @return True if both x < other.x and y < other.y.
      */
     bool operator<(const ivec2& other) const {
         return (x < other.x) && (y < other.y);
     }
 
     /**
      * @brief Component-wise greater-than comparison (>).
      *
      * @param other The vector to compare.
      * @return True if both x > other.x and y > other.y.
      */
     bool operator>(const ivec2& other) const {
         return (x > other.x) && (y > other.y);
     }
 
     /**
      * @brief Component-wise less-than-or-equal comparison (<=).
      *
      * @param other The vector to compare.
      * @return True if x <= other.x and y <= other.y.
      */
     bool operator<=(const ivec2& other) const {
         return (x <= other.x) && (y <= other.y);
     }
 
     /**
      * @brief Component-wise greater-than-or-equal comparison (>=).
      *
      * @param other The vector to compare.
      * @return True if x >= other.x and y >= other.y.
      */
     bool operator>=(const ivec2& other) const {
         return (x >= other.x) && (y >= other.y);
     }
 
     /**
      * @brief Returns component-wise greater-than result as a mask.
      *
      * @param other The vector to compare.
      * @return Vector where each component is 1 if this > other, else 0.
      */
     ivec2 greaterThan(const ivec2& other) const {
         return ivec2(x > other.x ? 1 : 0, y > other.y ? 1 : 0);
     }
 
     /**
      * @brief Returns component-wise less-than result as a mask.
      *
      * @param other The vector to compare.
      * @return Vector where each component is 1 if this < other, else 0.
      */
     ivec2 lessThan(const ivec2& other) const {
         return ivec2(x < other.x ? 1 : 0, y < other.y ? 1 : 0);
     }
 
     /**
      * @brief Returns component-wise greater-than-or-equal result as a mask.
      *
      * @param other The vector to compare.
      * @return Vector where each component is 1 if this >= other, else 0.
      */
     ivec2 greaterThanOrEqual(const ivec2& other) const {
         return ivec2(x >= other.x ? 1 : 0, y >= other.y ? 1 : 0);
     }
 
     /**
      * @brief Returns component-wise less-than-or-equal result as a mask.
      *
      * @param other The vector to compare.
      * @return Vector where each component is 1 if this <= other, else 0.
      */
     ivec2 lessThanOrEqual(const ivec2& other) const {
         return ivec2(x <= other.x ? 1 : 0, y <= other.y ? 1 : 0);
     }
 
     /**
      * @brief Returns the component-wise minimum.
      *
      * @param other The vector to compare.
      * @return New ivec2 with min(x, other.x), min(y, other.y).
      */
     ivec2 min(const ivec2& other) const {
         return ivec2(x < other.x ? x : other.x,
                      y < other.y ? y : other.y);
     }
 
     /**
      * @brief Returns the component-wise maximum.
      *
      * @param other The vector to compare.
      * @return New ivec2 with max(x, other.x), max(y, other.y).
      */
     ivec2 max(const ivec2& other) const {
         return ivec2(x > other.x ? x : other.x,
                      y > other.y ? y : other.y);
     }
 
     /**
      * @brief Clamps each component between given min and max vectors.
      *
      * @param minVec Vector of minimum allowed values.
      * @param maxVec Vector of maximum allowed values.
      * @return New ivec2 with clamped components.
      */
     ivec2 clamp(const ivec2& minVec, const ivec2& maxVec) const {
         return ivec2(
             x < minVec.x ? minVec.x : (x > maxVec.x ? maxVec.x : x),
             y < minVec.y ? minVec.y : (y > maxVec.y ? maxVec.y : y)
         );
     }
 
     /**
      * @brief Component-wise multiplication.
      *
      * @param other The vector to multiply with.
      * @return New ivec2 where each component is multiplied.
      */
     ivec2 multiply(const ivec2& other) const {
         return ivec2(x * other.x, y * other.y);
     }
 
     /**
      * @brief Converts the vector to a human-readable string.
      *
      * @return Arduino String in the format "(x, y)".
      */
     String toString() const {
         String result = "(" + String(x) + ", " + String(y) + ")";
         return result;
     }
 
     /**
      * @brief Prints the vector to the Serial Monitor.
      *
      * @param newLine If true, appends a newline after printing.
      */
     void display(bool newLine = true) const {
         if (newLine) {
             Serial.println(this->toString());
         } else {
             Serial.print(this->toString());
         }
     }
 };
 
 #endif // IVEC2_HPP 