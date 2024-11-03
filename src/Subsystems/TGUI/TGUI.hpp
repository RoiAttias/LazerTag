#ifndef TGUI_HPP
#define TGUI_HPP

#include <functional>

#include "Utilities\ivec2.hpp"
#include "Utilities\HyperList.hpp"

//#define finalPosition position+offset
#define AUTO -1

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
    hold,
    drag,
    released,
    click,
    size
};

typedef std::function<void(ivec2, TouchStatus)> TouchEvent;

#include "Element.hpp"

//#include "Grid.hpp"
//#include "Activity.hpp"
#include "Screen.hpp"

#include "Touch.hpp"



//latest touch
#endif