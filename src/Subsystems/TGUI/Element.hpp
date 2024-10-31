#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "TGUI.hpp"

/**
 * @brief The Element class serves as a base class for GUI elements.
 * 
 * This class manages the position, scale, and visibility of GUI elements. 
 * It supports touch events with configurable handlers and includes 
 * virtual functions for rendering and range checking. 
 * 
 * Key Features:
 * - **Position and Scale**: Uses `ivec2` to manage the element's 
 *   position and dimensions.
 * - **Visibility**: Controls the rendering state of the element.
 * - **Press Events**: Allows enabling/disabling of touch events 
 *   and setting a touch event handler function.
 * - **Virtual Functions**: Requires derived classes to implement 
 *   specific rendering logic.
 * - **Range Checking**: Provides methods to determine if a point 
 *   falls within the element's boundaries.
 * 
 * This class is designed for extensibility, allowing derived classes 
 * to implement custom behavior while providing foundational properties 
 * and methods for GUI elements.
 */
class Element {
protected:
    ivec2 position; // Position of the element
    ivec2 scale; // Scale of the element
    ivec2 offset = ivec2(0,0); // Offset the position of the element
    //ivec2 renderPosition, renderScale; // Position and scale for rendering
    bool visible; // Visibility flag

    bool OnTouch_enable; // Flag to enable touch events
    TouchEvent OnTouch_handler; // Press event handler

public:
    const int ID = newElementID(); // Unique ID of the element

    // Constructors
    /**
     * @brief Constructor of Element
     * 
     * @param position The position of the element on the X and Y axis.
     * @param scale The width and height of the element.
     * @param visible Enable rendering for the element.
     * @param OnTouch_enable Enable or disable the touch event for the element.
     * @param OnTouch_handler Pointer to the function to handle the touch event.
     */
    Element(ivec2 position = ivec2(), ivec2 scale = ivec2(), bool visible = true,
        bool OnTouch_enable = false, TouchEvent OnTouch_handler = nullptr)
        : position(position), scale(scale), visible(visible),
        OnTouch_enable(OnTouch_enable), OnTouch_handler(OnTouch_handler) {}
    
    /**
     * @brief Constructor of Element
     * 
     * @param xPosition The position of the element on the X axis.
     * @param yPosition The position of the element on the Y axis.
     * @param width The scale of the element on the X axis.
     * @param height The scale of the element on the Y axis.
     * @param visible Enable rendering for the element.
     * @param OnTouch_enable Enable or disable the touch event for the element.
     * @param OnTouch_handler Pointer to the function to handle the touch event.
     */
    Element(int xPosition = 0, int yPosition = 0, int width = 0, int height = 0, bool visible = true,
        bool OnTouch_enable = false, TouchEvent OnTouch_handler = nullptr)
        :Element(ivec2(xPosition, yPosition), ivec2(width, height), visible, OnTouch_enable, OnTouch_handler) {}

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
        return position+offset;
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
     * @brief Enable or disable the touch event for the element.
     * 
     * @param enable True to enable the touch event, false to disable it.
     */
    virtual void OnTouch_enable(bool enable) {
        OnTouch_enable = enable;
    }

    /**
     * @brief Set the touch event handler function.
     * 
     * @param handler Pointer to the function to handle the touch event.
     */
    virtual void OnTouch_setHandler(TouchEvent handler) {
        OnTouch_handler = handler;
    }

    /**
     * @brief Execute the touch event handler if enabled.
     * 
     * @param point The point where the touch occurred.
     */
    virtual void OnTouch_execute(ivec2 point, TouchStatus touchStatus) {
        if (OnTouch_enable && OnTouch_handler && TouchStatus::ready) {
            OnTouch_handler(point,touchStatus);
        }
    }
};

#endif // ELEMENT_HPP