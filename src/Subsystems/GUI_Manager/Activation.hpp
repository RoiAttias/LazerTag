#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include "GUI_Manager.hpp"
#include <Arduino.h>

#include "Subsystems/TGUI/TFT_eSPI_Elements/Button.hpp"


class Activation : public Activity {
public:
    // Elements
    Text text;
    Button plus_button;
    Button minus_button;
    Button story_button;

    // Variables
    int counter = 0;

    // Constructor
    Activation(TouchEvent up, TouchEvent down, TouchEvent story) : Activity(),
        text(Element(TGUI_AUTO, TGUI_AUTO, this->scale, true, true), "0", TFT_WHITE, 2, TR_DATUM, 2.0f, &FreeSans9pt7b),
        plus_button(Element(ivec2(60,200),TGUI_AUTO,ivec2(100,50),true,true,true,up), "+", TFT_BLACK, TFT_GREEN, TFT_WHITE, 2, 1.0f, 10, FF10),
        minus_button(Element(ivec2(160,200),TGUI_AUTO,ivec2(100,50),true,true,true,down), "-", TFT_BLACK, TFT_RED, TFT_WHITE, 2, 1.0f, 10, FF10),
        story_button(Element(ivec2(80,300),TGUI_AUTO,ivec2(160,50),true,true,true,story), "Story Time", 
                    TFT_GOLD, TFT_DARKCYAN, TFT_GOLD, 1, 1.0f, 0, &FreeSerifBoldItalic24pt7b)
    {
        // Activity settings
        renderAlways = false;

        // Add all Rectangle elements to the array
        Element* elems[] = {new Background(TFT_PURPLE), &text, &plus_button, &minus_button, &story_button};
        elements.addFromArray(elems, 5);
    }

    Viewport render(const Viewport &viewport) override {
        update();
        return Activity::render(viewport);
    }

    void update() {
        text.content = String(counter);
    }
};

void up(ivec2 point, TouchStatus touchStatus);
void down(ivec2 point, TouchStatus touchStatus);
void story(ivec2 point, TouchStatus touchStatus);

Activation * activation = new Activation(up, down, story);

void up(ivec2 point, TouchStatus touchStatus) {
    if (touchStatus == TouchStatus_PRESS) {
        activation->counter++;
        activation->callRender();
    }
}

void down(ivec2 point, TouchStatus touchStatus) {
    if (touchStatus == TouchStatus_PRESS) {
        activation->counter--;
        activation->callRender();
    }
}

void story(ivec2 point, TouchStatus touchStatus) {
    if (touchStatus == TouchStatus_PRESS) {
        switchActivity(3);
    }
}

#endif // ACTIVATION_HPP
