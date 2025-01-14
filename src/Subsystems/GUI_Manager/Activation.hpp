#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include "GUI_Manager.hpp"
#include <Arduino.h>

class Activation : public Activity {
private:
    Rectangle e1;
public:
    Activation() : Activity(TGUI_AUTO, TGUI_AUTO),
        e1(Element(ivec2(0,0), TGUI_AUTO, ivec2(10,240)), TFT_RED, TFT_BLACK, true, 10)
    {
        Element* elems[] = {&e1};
        elements.addFromArray(elems,1);
    }
};


#endif // ACTIVATION_HPP