#ifndef TOUCHEVENT_HPP
#define TOUCHEVENT_HPP

#include "TGUI.hpp"

enum TouchStatus {
    
};

class TouchEvent {
protected:
    ivec2 currentPosition, startPosition, endPosition;


    bool OnClick_enable; // Flag to enable click events
    std::function<void(ivec2)> OnClick_handler; // Click event handler

public:
    /*
    * get vector
    * get latest vector
    */
    const int ID = newElementID(); // Unique ID of the element

    // Constructors
    /**
     * @brief Constructor of Element
     * 
     * @param position The position of the element on the X and Y axis.
     * @param scale The width and height of the element.
     * @param visible Enable rendering for the element.
     * @param OnClick_enable Enable or disable the click event for the element.
     * @param OnClick_handler Pointer to the function to handle the click event.
     */
    Element(ivec2 position = ivec2(), ivec2 scale = ivec2(), bool visible = true, bool OnClick_enable = false, std::function<void(ivec2)> OnClick_handler = nullptr)
        : position(position), scale(scale), visible(visible), OnClick_enable(OnClick_enable), OnClick_handler(OnClick_handler) {}
    
    /**
     * @brief Constructor of Element
     * 
     * @param xPosition The position of the element on the X axis.
     * @param yPosition The position of the element on the Y axis.
     * @param width The scale of the element on the X axis.
     * @param height The scale of the element on the Y axis.
     * @param visible Enable rendering for the element.
     * @param OnClick_enable Enable or disable the click event for the element.
     * @param OnClick_handler Pointer to the function to handle the click event.
     */
    Element(int xPosition, int yPosition, int width, int height, bool visible = true, bool OnClick_enable = false, std::function<void(ivec2)> OnClick_handler = nullptr)
        :Element(ivec2(xPosition, yPosition), ivec2(width, height), visible, OnClick_enable, OnClick_handler) {}

    // Getters
    /**
     * @brief Get the position of the element.
     * 
     * @return The position of the element.
     */
    virtual ivec2 getPosition() const {
        return position;
    }

    /**
     * @brief Get the scale of the element.
     * 
     * @return The scale of the element.
     */
    virtual ivec2 getScale() const {
        return scale;
    }

    /**
     * @brief Get the offset of the element.
     * 
     * @return The offset of the element.
     */
    virtual ivec2 getOffset() const {
        return offset;
    }

    /**
     * @brief Get the final position of the element.
     * 
     * @return Position + Offset.
     */
    virtual ivec2 getFinalPosition() const {
        return finalPosition;
    }

    /**
     * @brief Get the visibility of the element.
     * 
     * @return True if rendering is enabled for the element.
     */
    virtual bool isVisible() const {
        return visible;
    }
    
    // Setters
    /**
     * @brief Set the position of the element.
     * 
     * @param position The new position for the element.
     */
    virtual void setPosition(ivec2 position) {
        this->position = position;
    }

    /**
     * @brief Set the scale of the element.
     * 
     * @param scale The new scale for the element.
     */
    virtual void setScale(ivec2 scale) {
        this->scale = scale;
    }

    /**
     * @brief Set the offset of the element.
     * 
     * @param offset The new offset for the element.
     */
    virtual void setOffset(ivec2 offset) {
        this->offset = offset;
    }

    /**
     * @brief Set the visibility of the element.
     * 
     * @param visible Enable rendering for the element.
     */
    virtual void setVisible(bool visible) {
        this->visible = visible;
    }

    // Functions to be overridden
    /**
     * @brief Pure virtual function for rendering the element.
     */
    virtual void render() = 0;

    // Calculations
    /**
     * @brief Check if a point is within the element's range.
     * 
     * @param x The x-coordinate of the point.
     * @param y The y-coordinate of the point.
     * @return True if the point is within range, false otherwise.
     */
    virtual bool inRange(int x, int y) const {
        return (x >= getFinalPosition().x && x <= getFinalPosition().x + scale.x &&
                y >= getFinalPosition().y && y <= getFinalPosition().y + scale.y);
    }

    /**
     * @brief Check if a point is within the element's range.
     * 
     * @param point The ivec2 to check.
     * @return True if the point is within range, false otherwise.
     */
    virtual bool inRange(const ivec2 point) const {
        return inRange(point.x, point.y);
    }

    /**
     * @brief Check if an element crosses the element's range.
     * 
     * @param eptr Pointer of the element to check.
     * @return True if one if the element's edges is within range, false otherwise.
     */
    virtual bool inRange(const Element * eptr) const {
        ivec2 e_scale = eptr->getScale();
        ivec2 px0y0 = eptr->getFinalPosition();
        ivec2 px0y1 = eptr->getFinalPosition() + ivec2(e_scale.y);
        ivec2 px1y0 = eptr->getFinalPosition() + ivec2(e_scale.x);
        ivec2 px1y1 = eptr->getFinalPosition() + e_scale;
        return inRange(px0y0) || inRange(px0y1) || inRange(px1y0) || inRange(px1y1);
    }

    // Events
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
    virtual void OnClick_setHandler(std::function<void(ivec2)> handler) {
        OnClick_handler = handler;
    }

    /**
     * @brief Execute the click event handler if enabled.
     * 
     * @param point The point where the click occurred.
     */
    virtual void OnClick_execute(ivec2 point) {
        if (OnClick_enable && OnClick_handler) {
            OnClick_handler(point);
        }
    }    
};

#endif // TOUCHEVENT_HPP