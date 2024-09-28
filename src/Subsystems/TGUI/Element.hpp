#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "TGUI.hpp"

/**
 * @brief The Element class serves as a base class for GUI elements.
 * 
 * This class manages the position, scale, and visibility of GUI elements. 
 * It supports click events with configurable handlers and includes 
 * virtual functions for rendering and range checking. 
 * 
 * Key Features:
 * - **Position and Scale**: Uses `ivec2` to manage the element's 
 *   position and dimensions.
 * - **Visibility**: Controls the rendering state of the element.
 * - **Click Events**: Allows enabling/disabling of click events 
 *   and setting a click event handler function.
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
    ivec2 position, scale; // Position and scale of the element
    bool visible; // Visibility flag

    bool OnClick_enable; // Flag to enable click events
    std::function<void(ivec2)> OnClick_handler; // Click event handler

public:
    // Constructors
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
    Element(int xPosition, int yPosition, int width, int height, bool visible = true,
        bool OnClick_enable = false, std::function<void(ivec2)> OnClick_handler = nullptr)
        : position(xPosition, yPosition), scale(width, height), visible(visible),
          OnClick_enable(OnClick_enable), OnClick_handler(OnClick_handler) {}

    /**
     * @brief Constructor of Element
     * 
     * @param position The position of the element on the X and Y axis.
     * @param scale The width and height of the element.
     * @param visible Enable rendering for the element.
     * @param OnClick_enable Enable or disable the click event for the element.
     * @param OnClick_handler Pointer to the function to handle the click event.
     */
    Element(ivec2 position, const ivec2& scale, bool visible = true,
        bool OnClick_enable = false, std::function<void(ivec2)> OnClick_handler = nullptr)
        : position(position), scale(scale), visible(visible),
          OnClick_enable(OnClick_enable), OnClick_handler(OnClick_handler) {}
        
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
     * @return bool True if the point is within range, false otherwise.
     */
    virtual bool inRange(int x, int y) const {
        return (x >= position.x && x <= position.x + scale.x &&
                y >= position.y && y <= position.y + scale.y);
    }

    /**
     * @brief Check if a point is within the element's range.
     * 
     * @param point The ivec2 to check.
     * @return bool True if the point is within range, false otherwise.
     */
    virtual bool inRange(const ivec2 point) const {
        return inRange(point.x, point.y);
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

#endif // ELEMENT_HPP