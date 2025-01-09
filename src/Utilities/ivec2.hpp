#ifndef IVEC2_HPP
#define IVEC2_HPP

#include <Arduino.h>
#include "vec2.hpp"

/**
 * @brief Struct representing a point (or vector) in 2D space with integer coordinates.
 * 
 * This struct is designed for use in Arduino projects, providing a simple and efficient way 
 * to represent 2D vectors with integer values. It includes operations such as addition, 
 * subtraction, scalar multiplication and division, normalization, and dot product, making it useful 
 * for graphics, physics calculations, and general vector math.
 */
struct ivec2
{
    int x; //< The x-coordinate of the vector.
    int y; //< The y-coordinate of the vector.

    /**
     * @brief Constructor for ivec2.
     * 
     * @param x The x-coordinate of the vector (default is 0).
     * @param y The y-coordinate of the vector (default is 0).
     */
    ivec2(int x = 0, int y = 0) : x(x), y(y) {}

    /**
     * @brief Constructor for ivec2.
     * 
     * @param v The floating vector.
     */
    ivec2(vec2 v)
    {
        this->x = int(v.x);
        this->y = int(v.y);
    }

    /**
     * @brief Addition assignment operator (+=).
     * 
     * @param other The vector to add.
     * @return A reference to the modified vector.
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
     * @return A reference to the modified vector.
     */
    ivec2& operator-=(const ivec2& other) {
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
    ivec2& operator*=(int scalar) {
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
    ivec2& operator/=(int scalar) {
        if (scalar != 0) { // Avoid division by zero
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
    bool operator==(const ivec2& other) const {
        return (x == other.x) && (y == other.y);
    }

    /**
     * @brief Inequality operator (!=).
     * 
     * @param other The vector to compare against.
     * @return True if the vectors are not equal, false otherwise.
     */
    bool operator!=(const ivec2& other) const {
        return !(*this == other);
    }

    /**
     * @brief Addition operator (+).
     * 
     * @param other The vector to add.
     * @return A new vector that is the sum of the two vectors.
     */
    ivec2 operator+(const ivec2& other) const {
        return ivec2(x + other.x, y + other.y);
    }

    /**
     * @brief Subtraction operator (-).
     * 
     * @param other The vector to subtract.
     * @return A new vector that is the difference of the two vectors.
     */
    ivec2 operator-(const ivec2& other) const {
        return ivec2(x - other.x, y - other.y);
    }

    /**
     * @brief Scalar multiplication operator (*).
     * 
     * @param scalar The scalar to multiply with.
     * @return A new vector that is the product of the vector and scalar.
     */
    ivec2 operator*(int scalar) const {
        return ivec2(x * scalar, y * scalar);
    }

    /**
     * @brief Scalar division operator (/).
     * 
     * @param scalar The scalar to divide by.
     * @return A new vector that is the quotient of the vector and scalar.
     */
    ivec2 operator/(int scalar) const {
        if (scalar != 0) {
            return ivec2(x / scalar, y / scalar);
        }
        return ivec2(0, 0); // Handle division by zero
    }

    ivec2 operator>(const ivec2& other) const {
        return ivec2(x > other.x ? 1 : 0, y > other.y ? 1 : 0);
    }

    ivec2 operator<(const ivec2& other) const {
        return ivec2(x < other.x ? 1 : 0, y < other.y ? 1 : 0);
    }

    ivec2 operator>=(const ivec2& other) const {
        return ivec2(x >= other.x ? 1 : 0, y >= other.y ? 1 : 0);
    }

    ivec2 operator<=(const ivec2& other) const {
        return ivec2(x <= other.x ? 1 : 0, y <= other.y ? 1 : 0);
    }

    ivec2 min(const ivec2& other) const {
        return ivec2(x < other.x ? x : other.x, y < other.y ? y : other.y);
    }

    ivec2 max(const ivec2& other) const {
        return ivec2(x > other.x ? x : other.x, y > other.y ? y : other.y);
    }

    /**
     * @brief Calculates the dot product with another vector.
     * 
     * @param other The vector to clamp with.
     * @return Vector with bigger x and smaller y.
     */
    ivec2 clamp(const ivec2& other) const {
        return ivec2(x > other.x ? x : other.x, y < other.y ? y : other.y);
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

#endif // IVEC2_HPP