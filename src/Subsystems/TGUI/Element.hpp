#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "TGUI.hpp"

class Element {
protected:
    int xPosition, yPosition, width, height;
    bool visible;

    Point renderPosition; // To store the oosition adjusted for rendering

    bool OnClick_enable;
    void (*OnClick_handler)(Point);

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

public:
    Element(int xPosition, int yPosition, int width, int height)
        : xPosition(xPosition), yPosition(yPosition),
          width(width), height(height),
          visible(true),
          OnClick_enable(false), OnClick_handler(nullptr),
          renderPosition(xPosition, yPosition) {} // Initialize renderPosition

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
        setRenderPositionByOffset(newX-xPosition,newY-yPosition); // Update renderPosition when position changes
        xPosition = newX;
        yPosition = newY;
    }

    /**
     * @brief Get the position of the element as a Point.
     * 
     * @return Point The position of the element.
     */
    virtual Point getPosition() const {
        return Point(xPosition, yPosition);
    }

    /**
     * @brief Get the width of the element.
     * 
     * @return int The width of the element.
     */
    virtual int getWidth() const {
        return width;
    }

    /**
     * @brief Get the height of the element.
     * 
     * @return int The height of the element.
     */
    virtual int getHeight() const {
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
     * @brief Get the visibility of the element.
     * 
     * @return isVisible True to make the element visible, false to hide it.
     */
    virtual void isVisible() {
        return visible;
    }


    /**
     * @brief Set the render Position by adding an offset to the element's position.
     * 
     * @param xOffset The x-coordinate offset.
     * @param yOffset The y-coordinate offset.
     */
    virtual void setRenderPositionByOffset(int xOffset, int yOffset) {
        renderPosition = Point(xPosition + xOffset, yPosition + yOffset);
    }

    /**
     * @brief Set the render Position directly from a Point.
     * 
     * @param point The new render Position.
     */
    virtual void setRenderPositionByPoint(Point point) {
        renderPosition = point;
    }

    /**
     * @brief Get the render Position of the element.
     * 
     * @return Point The render Position of the element.
     */
    virtual Point getRenderPosition() const {
        return renderPosition;
    }

    /**
     * @brief Check if a point is within the element's range.
     * 
     * @param x The x-coordinate of the point.
     * @param y The y-coordinate of the point.
     * @return bool True if the point is within range, false otherwise.
     */
    virtual bool inRange(int x, int y) const {
        return (x >= renderPosition.x && x <= renderPosition.x + width &&
                y >= renderPosition.y && y <= renderPosition.y + height);
    }

    /**
     * @brief Check if a point is within the element's range.
     * 
     * @param point The Point to check.
     * @return bool True if the point is within range, false otherwise.
     */
    virtual bool inRange(const Point point) const {
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
