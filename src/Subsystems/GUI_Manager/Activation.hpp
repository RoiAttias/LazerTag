#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include "GUI_Manager.hpp"
#include <Arduino.h>


void button(ivec2 point, TouchStatus touchStatus) {
    
        Serial.println("Button pressed");
//        text.content = String(text.content.toInt() + 1);
}


class Activation : public Activity {
public:
    // Elements
    Rectangle rect;
    Text text;

    // Constructor
    Activation() : Activity(TGUI_AUTO, TGUI_AUTO,true, true, true,button),
        rect(Element(ivec2(random(0, 240),random(0, 320)), TGUI_AUTO, ivec2(50,100)), TFT_RED, TFT_BLACK, true, true, 15),
        text(Element(), "0", TFT_WHITE, 1, 2, 2.0f, &FreeSans9pt7b)
    {
        // Add all Rectangle elements to the array
        Element* elems[] = {&rect, &text};
        elements.addFromArray(elems, 2);
    }
};

#endif // ACTIVATION_HPP
