#ifndef TGUI_HPP
#define TGUI_HPP

#include <functional>

#include "Utilities/MoreMath.hpp"
#include "Utilities/ivec2.hpp"
#include "Utilities/vec2.hpp"

#include "Utilities/HyperList.hpp"

#include "Components/Pushbutton/Pushbutton.hpp"


#define TGUI_AUTO -1

// Element IDs
int elementIDs = 0; // The amount of element created

/**
 * @brief Gives each element a unique ID number.
 * Helps when comparing 2 similar elements.
 * @return New ID number for an element.
 */
int newElementID()
{
    return ++elementIDs;
}

enum TouchStatus : byte{
    READY,
    PRESS,
    RELEASE,
    SWIPE,
    HOLD,
    DRAG,
    TouchStatus_size
};

#define ENABLE_ALL 1
#define ENABLE_PRESS 1<<TouchStatus::PRESS
#define ENABLE_RELEASE 1<<TouchStatus::RELEASE
#define ENABLE_SWIPE 1<<TouchStatus::SWIPE
#define ENABLE_HOLD 1<<TouchStatus::HOLD
#define ENABLE_DRAG 1<<TouchStatus::DRAG



typedef std::function<void(ivec2, TouchStatus, uint32_t)> TouchEvent;

#include "Element.hpp"
#include "Activity.hpp"
#include "Screen.hpp"
#include "Touch.hpp"


#define LUMINAUI_USE_TFT_ESPI
#ifdef LUMINAUI_USE_TFT_ESPI
    #include "TFT_eSPI_Elements/TFT_eSPI_Elements.hpp"
#endif

#endif // TGUI_HPP