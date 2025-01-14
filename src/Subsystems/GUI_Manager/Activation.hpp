#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include "GUI_Manager.hpp"
#include <Arduino.h>

class Activation : public Activity {
private:
    // Array of Rectangle objects with randomized positions, sizes, border colors, border flags, and corner radii
    Rectangle e1, e2, e3, e4, e5, e6, e7, e8, e9, e10;
public:
    Activation() : Activity(TGUI_AUTO, TGUI_AUTO),
        e1(Element(ivec2(random(0, 240), random(0, 320)), TGUI_AUTO, ivec2(random(10, 100), random(10, 100))), TFT_RED, TFT_BLACK, random(0, 2), random(5, 20)),
        e2(Element(ivec2(random(0, 240), random(0, 320)), TGUI_AUTO, ivec2(random(10, 100), random(10, 100))), TFT_BLUE, TFT_WHITE, random(0, 2), random(5, 20)),
        e3(Element(ivec2(random(0, 240), random(0, 320)), TGUI_AUTO, ivec2(random(10, 100), random(10, 100))), TFT_GREEN, TFT_BLACK, random(0, 2), random(5, 20)),
        e4(Element(ivec2(random(0, 240), random(0, 320)), TGUI_AUTO, ivec2(random(10, 100), random(10, 100))), TFT_YELLOW, TFT_BLACK, random(0, 2), random(5, 20)),
        e5(Element(ivec2(random(0, 240), random(0, 320)), TGUI_AUTO, ivec2(random(10, 100), random(10, 100))), TFT_CYAN, TFT_BLACK, random(0, 2), random(5, 20)),
        e6(Element(ivec2(random(0, 240), random(0, 320)), TGUI_AUTO, ivec2(random(10, 100), random(10, 100))), TFT_MAGENTA, TFT_BLACK, random(0, 2), random(5, 20)),
        e7(Element(ivec2(random(0, 240), random(0, 320)), TGUI_AUTO, ivec2(random(10, 100), random(10, 100))), TFT_ORANGE, TFT_BLACK, random(0, 2), random(5, 20)),
        e8(Element(ivec2(random(0, 240), random(0, 320)), TGUI_AUTO, ivec2(random(10, 100), random(10, 100))), TFT_PINK, TFT_BLACK, random(0, 2), random(5, 20)),
        e9(Element(ivec2(random(0, 240), random(0, 320)), TGUI_AUTO, ivec2(random(10, 100), random(10, 100))), TFT_PURPLE, TFT_BLACK, random(0, 2), random(5, 20)),
        e10(Element(ivec2(random(0, 240), random(0, 320)), TGUI_AUTO, ivec2(random(10, 100), random(10, 100))), TFT_BROWN, TFT_BLACK, random(0, 2), random(5, 20))
    {
        // Add all Rectangle elements to the array
        Element* elems[] = {&e1, &e2, &e3, &e4, &e5, &e6, &e7, &e8, &e9, &e10};
        elements.addFromArray(elems, 10);
    }
};

#endif // ACTIVATION_HPP
