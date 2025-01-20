#ifndef VEC2_HPP
#define VEC2_HPP

#include <Arduino.h>
#include "ivec2.hpp"

/**
 * @brief Struct representing a point (or vector) in 2D space, similar to OpenGL vectors.
 * 
 * This struct is designed for use in Arduino projects, providing a simple and efficient way 
 * to represent 2D vectors. It includes operations such as addition, subtraction, 
 * scalar multiplication and division, normalization, and dot product, making it useful 
 * for graphics, physics calculations, and general vector math.
 */
struct vec2
{
    float x; //< The x-coordinate of the vector.
    float y; //< The y-coordinate of the vector.

    /**
     * @brief Constructor for vec2.
     * 
     * @param x The x-coordinate of the vector (default is 0.0).
     * @param y The y-coordinate of the vector (default is 0.0).
     */
    vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    /**
     * @brief Constructor for vec2 from an ivec2.
     * 
     * @param i The ivec2 to convert to a vec2.
     */
    vec2(ivec2 i) : x(i.x), y(i.y) {}

    /**
     * @brief Addition assignment operator (+=).
     * 
     * @param other The vector to add.
     * @return A reference to the modified vector.
     */
    vec2& operator+=(const vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    /**
     * @brief Subtraction assignment operator (-=).
     * 
     * @param other The vector to subtract.
     * @return A reference to the modified vector.
     */
    vec2& operator-=(const vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    /**
     * @brief Scalar multiplication assignment operator (*=).
     * 
     * @param scalar The scalar to multiply with.
     * @return A reference to the modified vector.
     */
    vec2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    /**
     * @brief Scalar division assignment operator (/=).
     * 
     * @param scalar The scalar to divide by.
     * @return A reference to the modified vector.
     */
    vec2& operator/=(float scalar) {
        if (scalar != 0.0f) { // Avoid division by zero
            x /= scalar;
            y /= scalar;
        }
        return *this;
    }

    /**
     * @brief Equality operator (==).
     * 
     * @param other The vector to compare against.
     * @return True if the vectors are equal, false otherwise.
     */
    bool operator==(const vec2& other) const {
        return (x == other.x) && (y == other.y);
    }

    /**
     * @brief Inequality operator (!=).
     * 
     * @param other The vector to compare against.
     * @return True if the vectors are not equal, false otherwise.
     */
    bool operator!=(const vec2& other) const {
        return !(*this == other);
    }

    /**
     * @brief Addition operator (+).
     * 
     * @param other The vector to add.
     * @return A new vector that is the sum of the two vectors.
     */
    vec2 operator+(const vec2& other) const {
        return vec2(x + other.x, y + other.y);
    }

    /**
     * @brief Subtraction operator (-).
     * 
     * @param other The vector to subtract.
     * @return A new vector that is the difference of the two vectors.
     */
    vec2 operator-(const vec2& other) const {
        return vec2(x - other.x, y - other.y);
    }

    /**
     * @brief Scalar multiplication operator (*).
     * 
     * @param scalar The scalar to multiply with.
     * @return A new vector that is the product of the vector and scalar.
     */
    vec2 operator*(float scalar) const {
        return vec2(x * scalar, y * scalar);
    }

    /**
     * @brief Scalar division operator (/).
     * 
     * @param scalar The scalar to divide by.
     * @return A new vector that is the quotient of the vector and scalar.
     */
    vec2 operator/(float scalar) const {
        if (scalar != 0.0f) {
            return vec2(x / scalar, y / scalar);
        }
        return vec2(0.0f, 0.0f); // Handle division by zero
    }

    /**
     * @brief Negates this vector.
     * 
     * @return A new vector that is the negation of this vector.
     */
    vec2 operator-() const {
        return vec2(-x, -y);
    }

     /**
     * @brief Calculates the angle of the vector in radians.
     * 
     * @return The angle of the vector in radians.
     */
    float angle() const {
        return atan2(y, x);
    }

    /**
     * @brief Calculates the dot product with another vector.
     * 
     * @param other The vector to calculate the dot product with.
     * @return The dot product of the two vectors.
     */
    float dot(const vec2& other) const {
        return (x * other.x) + (y * other.y);
    }

    /**
     * @brief Calculates the cross product of this vector and another vector.
     * 
     * @param other The other vector.
     * @return The cross product of the two vectors.
     */
    float cross(const vec2& other) const {
        return x * other.y - y * other.x;
    }

    /**
     * @brief Calculates the distance to another vector.
     * 
     * @param other The vector to calculate the distance to with.
     * @return The distance between the two vectors.
     */
    float distanceTo(const vec2& other) const {
        return sqrt(((x - other.x) * (x - other.x)) + ((y - other.y) * (y - other.y)));
    }

    /**
     * @brief Calculates the magnitude (length) of the vector.
     * 
     * @return The magnitude of the vector.
     */
    float magnitude() const {
        return sqrt(x * x + y * y); // Using Arduino's sqrt()
    }

    /**
     * @brief Normalizes the vector (makes it unit length).
     * 
     * @return A new vector that is the normalized version of the original vector.
     */
    vec2 normalize() const {
        float mag = magnitude();
        if (mag > 0.0f) {
            return vec2(x / mag, y / mag);
        }
        return vec2(0.0f, 0.0f);
    }

    /**
     * @brief Rotates the vector by a given angle in radians.
     * 
     * @param angle The angle in radians to rotate the vector.
     * @return A new vector that is the rotated version of the original vector.
     */
    vec2 rotate(float angle) const {
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);
        return vec2(
            x * cosAngle - y * sinAngle,
            x * sinAngle + y * cosAngle
        );
    }

    /**
     * @brief Converts the vector to a string representation.
     * 
     * @return A string representing the vector in the format "(x, y)".
     */

    String toString() const {
        String result = "(";
        result += String(x);
        result += ", ";
        result += String(y);
        result += ")";
        return result;
    }

    /**
     * @brief Prints the vector on the Serial Monitor.
     */
    void display(bool newLine = true) const {
        if(newLine) {
            Serial.println(this->toString());
        }
        else {
            Serial.print(this->toString());
        }
    }
};

#endif // VEC2_HPP