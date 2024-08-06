#ifndef TOUCH_HPP
#define TOUCH_HPP

/**
 * @brief Struct representing a point in 2D space.
 */
struct Point
{
    int x; //< The x-coordinate of the point.
    int y; //< The y-coordinate of the point.

    /**
     * @brief Constructor for Point.
     * 
     * @param x The x-coordinate of the point. Default is -1.
     * @param y The y-coordinate of the point. Default is -1.
     */
    Point(int x = -1, int y = -1) : x(x), y(y) {}

    /**
     * @brief The equality operator
     * 
     * @param other const Point
     */
    bool operator==(const Point other) const {
        return (x == other.x) && (y == other.y);
    }

};

/**
 * @brief Class representing a Touch input.
 */
class Touch
{
protected:
    Point point; //< The current point of Touch input.

public:
    /**
     * @brief Get the current Touch point.
     * 
     * @return Point The current Touch point.
     */
    Point getPoint() { return point; }

    /**
     * @brief Get the address of the current Touch point.
     * 
     * @return Point* Pointer to the current Touch point.
     */
    Point* getPointAddress() { return &point; }

    /**
     * @brief Set the current Touch point using integer coordinates.
     * 
     * @param x The x-coordinate to set.
     * @param y The y-coordinate to set.
     */
    void setPoint(int x, int y) { point = Point(x, y); }

    /**
     * @brief Set the current Touch point using pointers to integer coordinates.
     * 
     * @param x Pointer to the x-coordinate to set. Should not be nullptr.
     * @param y Pointer to the y-coordinate to set. Should not be nullptr.
     */
    void setPoint(int* x, int* y) 
    {
        if (x != nullptr && y != nullptr) 
        {
            point = Point(*x, *y);
        }
    }
};


#endif // TOUCH_HPP
