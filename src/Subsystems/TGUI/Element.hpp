#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "TGUI.hpp"

class Element {
public:
    int xLocation, yLocation, width, height;
    bool visible;

private:
    bool OnClick_enable;
    void (*OnClick_handler)(Point);

public:
    Element(int xLocation, int yLocation, int width, int height)
        : xLocation(xLocation), yLocation(yLocation),
        width(width), height(height),
        visible(true),
        OnClick_enable(false), OnClick_handler(nullptr) {}

    /**
     * @brief Pure virtual function for rendering the element.
     */
    virtual void render() = 0;

    /**
     * @brief Set the position of the element.
     * 
     * @param newX The new x-coordinate of the element.
     * @param newY The new y-coordinate of the element.
     */
    virtual void setPosition(int newX, int newY) {
        xLocation = newX;
        yLocation = newY;
    }

    /**
     * @brief Get the position of the element as a Point.
     * 
     * @return Point The position of the element.
     */
    virtual Point getPosition() {
        return Point(xLocation, yLocation);
    }

    /**
     * @brief Set the size of the element.
     * 
     * @param newWidth The new width of the element.
     * @param newHeight The new height of the element.
     */
    virtual void setSize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;
    }

    /**
     * @brief Get the width of the element.
     * 
     * @return int The width of the element.
     */
    virtual int getWidth() {
        return width;
    }

    /**
     * @brief Get the height of the element.
     * 
     * @return int The height of the element.
     */
    virtual int getHeight() {
        return height;
    }

    /**
     * @brief Set the visibility of the element.
     * 
     * @param isVisible True to make the element visible, false to hide it.
     */
    virtual void setVisible(bool isVisible) {
        visible = isVisible;
    }

    /**
     * @brief Check if a point is within the element's range.
     * 
     * @param x The x-coordinate of the point.
     * @param y The y-coordinate of the point.
     * @return bool True if the point is within range, false otherwise.
     */
    virtual bool inRange(int x, int y) {
        return (x >= xLocation && x <= xLocation + width &&
                y >= yLocation && y <= yLocation + height);
    }

    /**
     * @brief Check if a point is within the element's range.
     * 
     * @param point The Point to check.
     * @return bool True if the point is within range, false otherwise.
     */
    virtual bool inRange(const Point point) {
        return inRange(point.x, point.y);
    }

    /**
     * @brief Enable or disable the click event for the element.
     * 
     * @param enable True to enable the click event, false to disable it.
     */
    virtual void OnClick_enableEvent(bool enable) {
        OnClick_enable = enable;
    }

    /**
     * @brief Set the click event handler function.
     * 
     * @param handler Pointer to the function to handle the click event.
     */
    virtual void OnClick_setHandler(void (*handler)(Point)) {
        OnClick_handler = handler;
    }

    /**
     * @brief Execute the click event handler if enabled.
     * 
     * @param point The point where the click occurred.
     */
    virtual void OnClick_execute(Point point) {
        if (OnClick_enable && OnClick_handler != nullptr) {
            OnClick_handler(point);
        }
    }
};

#endif // ELEMENT_HPP
