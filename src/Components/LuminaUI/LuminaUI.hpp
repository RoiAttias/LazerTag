#ifndef LUMINAUI_HPP
#define LUMINAUI_HPP

#include <Arduino.h>

#include "Utilities/ivec2.hpp"
#include "Utilities/vec2.hpp"

#include "Utilities/HyperList.hpp"

#include "Components/Pushbutton/Pushbutton.hpp"


const ivec2 LuminaUI_AUTO = ivec2(-1,-1); // Auto value for origin and scale

// Element IDs
int elementIDs = 0; // The amount of elements created

/**
 * @brief Gives each element a unique ID number.
 * Helps when comparing 2 similar elements.
 * @return New ID number for an element.
 */
int newElementID()
{
    return ++elementIDs;
}

/**
 * @brief Viewport struct for defining a rectangular area for rendering.
 */
struct Viewport
{
    ivec2 position; // Top left corner
    ivec2 scale; // Width and Height

    /**
     * @brief Constructor for Viewport.
     * @param position The top left corner of the viewport.
     * @param scale The width and height of the viewport.
     */
    Viewport(ivec2 position = ivec2(0, 0), ivec2 scale = ivec2(0, 0))
        : position(position), scale(scale) {}

    /**
     * @brief Get the center of the viewport.
     * @return The center of the viewport.
     */
    ivec2 center()
    {
        return position + scale / 2;
    }

    /**
     * @brief Check if a point is within the viewport.
     * @param point The point to check.
     * @return True if the point is within the viewport, false otherwise.
     */
    bool inRange(ivec2 point) const
    {
        return (point.x >= position.x && point.x <= position.x + scale.x &&
                point.y >= position.y && point.y <= position.y + scale.y);
    }

    bool inRange(const Viewport other) const
    {
        ivec2 e_scale = other.scale;    // Element scale
        ivec2 pLT= other.position; // Left Top
        ivec2 pLB = other.position + ivec2(e_scale.y); // Left Bottom
        ivec2 pRT = other.position + ivec2(e_scale.x); // Right Top
        ivec2 pRB = other.position + e_scale; // Right Bottom
        return inRange(pLT) || inRange(pLB) || inRange(pRT) || inRange(pRB);
    }

    /**
     * @brief Get the position after applying padding.
     * @param padding Padding values (left, top, right, bottom).
     * @return The position after applying padding.
     */
    ivec2 positionAfterPadding(int padding[4]) const
    {
        return position + ivec2(padding[0], padding[1]);
    }

    /**
     * @brief Get the scale after applying padding.
     * @param padding Padding values (left, top, right, bottom).
     * @return The scale after applying padding.
     */
    ivec2 scaleInsidePadding(int padding[4]) const
    {
        return scale - ivec2(padding[0], padding[1]) - ivec2(padding[2], padding[3]);
    }

    /**
     * @brief Get the viewport after applying padding.
     * @param padding Padding values (left, top, right, bottom).
     * @return The viewport after applying padding.
     */
    Viewport afterPadding(int padding[4]) const
    {
        return {positionAfterPadding(padding), scaleInsidePadding(padding)};
    }

    Viewport clamp(const Viewport &other) const
    {
        // Calculate the overlapping position and scale
        ivec2 newPos = position.max(other.position);
        ivec2 newEnd = position + scale;
        newEnd = newEnd.min(other.position + other.scale);
        ivec2 newScale = newEnd - newPos;

        // If there's no overlap, return an empty viewport
        if (newScale.x <= 0 || newScale.y <= 0)
        {
            return Viewport{ivec2(0, 0), ivec2(0, 0)};
        }

        // Return the intersected viewport
        return Viewport{newPos, newScale};
    }

    String toString() const
    {
        String result = "Viewport: {Position: " + position.toString() + " Scale: " + scale.toString() + "}";
        return result;
    }
};

/**
 * @brief Status of the touch event.
 */
enum TouchStatus : byte{
    TouchStatus_READY,
    TouchStatus_PRESS,
    TouchStatus_RELEASE,
    TouchStatus_HOLD,
    TouchStatus_DRAG,
    TouchStatus_size
};

String TouchStatus_strings[] = {
    "READY", "PRESS", "RELEASE", "HOLD", "DRAG", "size"
};

#define ENABLE_PRESS 1<<TouchStatus::TouchStatus_PRESS
#define ENABLE_RELEASE 1<<TouchStatus::TouchStatus_RELEASE
#define ENABLE_HOLD 1<<TouchStatus::TouchStatus_HOLD
#define ENABLE_DRAG 1<<TouchStatus::TouchStatus_DRAG
#define ENABLE_ALL 0xFF ^ 1<<TouchStatus::TouchStatus_READY // All except READY

struct TouchData
{
    TouchStatus status; // Current status of the touch
    ivec2 startPosition; // Start position when pressed
    ivec2 currentPosition; // Current position of the drag
    ivec2 endPosition; // End position when released
};

struct TouchDragData
{
    ivec2 startPosition; // Start position when pressed
    ivec2 currentPosition; // Current position of the drag
    ivec2 endPosition; // End position when released
};

#include "Element.hpp"
#include "Activity.hpp"
#include "Screen.hpp"
#include "Touch.hpp"

#include "Grid.hpp"
#include "ViewBox.hpp"

#ifdef LUMINAUI_USE_TFT_ESPI
    #include "TFT_eSPI_Elements/TFT_eSPI_Elements.hpp"
#endif

#endif // LUMINAUI_HPP