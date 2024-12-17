#ifndef TGUI_HPP
#define TGUI_HPP

#include <functional>

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
enum TouchStatus{
    ready,
    pressed,
    released,
    TouchStatus_size
};
/*
enum TouchStatus{
    ready,
    pressed,
    hold,
    released,
    drag,
    click,
    TouchStatus_size
};
*/


typedef std::function<void(ivec2, TouchStatus)> TouchEvent;

#include "Element.hpp"
#include "Activity.hpp"
#include "Screen.hpp"

//#include "Touch.hpp"

//#include "TFT_eSPI_Elements/TFT_eSPI_Element.h"

//latest touch
#endif // TGUI_HPP