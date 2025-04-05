#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "LuminaUI.hpp"
using TouchEvent = void(*)(ivec2, TouchStatus);

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
class Element
{
protected:
    bool _shouldRender; ///< Flag for rendering when called
public:

    // Base properties
    ivec2 origin;               // Origin position of the element - changed by programmer
    ivec2 offset; // Offset the position of the element - changed by containers
    ivec2 scale;                // Scale of the element
    bool visible;               // Visibility flag
    bool renderAlways;         // Flag to render the element without calls

    // Touch events
    bool OnTouch_enable;        // Flag to enable touch events
    TouchEvent OnTouch_handler; // Press event handler

    // Constructors
    /**
     * @brief Constructor of Element
     *
     * @param origin The origin position of the element on the X and Y axis - for programmer.
     * @param offset The offset of the position of the element on the X and Y axis - for containers.
     * @param scale The width and height of the element.
     * @param visible Enable rendering for the element.
     * @param renderAlways Flag to render the element without calls.
     * @param OnTouch_enable Enable or disable the touch event for the element.
     * @param OnTouch_handler Pointer to the function to handle the touch event.
     */
    Element(ivec2 origin = LuminaUI_AUTO, ivec2 offset = LuminaUI_AUTO, ivec2 scale = LuminaUI_AUTO, bool visible = true, bool renderAlways = true,
            bool OnTouch_enable = false, TouchEvent OnTouch_handler = nullptr)
        : origin(origin), offset(offset), scale(scale), visible(visible), renderAlways(renderAlways),
          OnTouch_enable(OnTouch_enable), OnTouch_handler(OnTouch_handler), _shouldRender(true) {
            if (origin == LuminaUI_AUTO) {
                origin = ivec2(0, 0);
            }
          }

    /**
     * @brief Constructor of Element
     *
     * @param xOrigin The origin position of the element on the X axis - for programmer.
     * @param yOrigin The origin position of the element on the Y axis - for programmer.
     * @param xOffset The offset of the position of the element on the X axis - for containers.
     * @param yOffset The offset of the position of the element on the Y axis - for containers.
     * @param width The scale of the element on the X axis.
     * @param height The scale of the element on the Y axis.
     * @param visible Enable rendering for the element.
     * @param renderAlways Flag to render the element without calls.
     * @param OnTouch_enable Enable or disable the touch event for the element.
     * @param OnTouch_handler Pointer to the function to handle the touch event.
     */
    Element(int xOrigin, int yOrigin, int xOffset, int yOffset, int width, int height, bool visible = true,
            bool renderAlways = false, bool OnTouch_enable = false, TouchEvent OnTouch_handler = nullptr)
        : Element(ivec2(xOrigin, yOrigin), ivec2(xOffset, yOffset), ivec2(width, height), visible, renderAlways,
            OnTouch_enable, OnTouch_handler) {}

    // Getters
    /**
     * @brief Get the final position of the element.
     *
     * @return Origin + Offset.
     */
    virtual ivec2 getPosition() const
    {
        return origin + offset;
    }

    /**
     * @brief Get the final size of the element.
     *
     * @return Scale.
     */
    virtual ivec2 getSize() const
    {
        return scale;
    }
    
    /**
     * @brief Get the final viewport of the element.
     *
     * @return Viewport of the element
     */
    virtual Viewport getViewport() const
    {
        return Viewport(getPosition(), scale);
    }

    bool shouldRender() {
        return _shouldRender || renderAlways;
    }

    // Setters
    void callRender() {
        _shouldRender = true;
    }

    // Functions to be overridden
    /**
     * @brief Virtual function for rendering the element.
     * Use the function to reset the _shouldRender flag and to get the clamped viewport.
     * @param viewport The viewport to render the element in.
     * @return The clamped viewport of the element.
     */
    virtual Viewport render(const Viewport &viewport) {
        _shouldRender = false;
        return getViewport().clamp(viewport);
    }

    // Calculations
    /**
     * @brief Check if a point is within the element's viewport range.
     *
     * @param point The ivec2 to check.
     * @return True if the point is within range, false otherwise.
     */
    virtual bool inRange(const ivec2 point) const
    {
        return getViewport().inRange(point);
    }
    
    /**
     * @brief Check if a point is within the element's viewport range.
     *
     * @param x The x-coordinate of the point.
     * @param y The y-coordinate of the point.
     * @return True if the point is within range, false otherwise.
     */
    virtual bool inRange(int x, int y) const
    {
        return inRange(ivec2(x, y));
    }

    /**
     * @brief Check if an element crosses the element's viewport range.
     *
     * @param eptr Pointer of the element to check.
     * @return True if one if the element's edges is within range, false otherwise.
     */
    virtual bool inRange(const Element *eptr) const
    {
        ivec2 e_scale = eptr->scale;    // Element scale
        ivec2 pLT= eptr->getPosition(); // Left Top
        ivec2 pLB = eptr->getPosition() + ivec2(0,e_scale.y); // Left Bottom
        ivec2 pRT = eptr->getPosition() + ivec2(e_scale.x,0); // Right Top
        ivec2 pRB = eptr->getPosition() + e_scale; // Right Bottom
        return inRange(pLT) || inRange(pLB) || inRange(pRT) || inRange(pRB);
    }

    // Events
    /**
     * @brief Enable or disable the touch event for the element.
     *
     * @param enable True to enable the touch event, false to disable it.
     */
    virtual void OnTouch_setEnable(bool enable)
    {
        OnTouch_enable = enable;
    }

    /**
     * @brief Set the touch event handler function.
     *
     * @param handler Pointer to the function to handle the touch event.
     */
    virtual void OnTouch_setHandler(TouchEvent handler)
    {
        OnTouch_handler = handler;
    }

    /**
     * @brief Execute the touch event handler if enabled.
     *
     * @param point The point where the touch occurred.
     */
    virtual void OnTouch_execute(ivec2 point, TouchStatus touchStatus)
    {
        if (OnTouch_enable && OnTouch_handler && inRange(point) && touchStatus < TouchStatus::TouchStatus_size)
        {
            OnTouch_handler(point, touchStatus);
        }
    }
};

#endif // ELEMENT_HPP