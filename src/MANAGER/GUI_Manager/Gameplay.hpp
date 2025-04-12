#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "GUI_Manager.hpp"
#include <Arduino.h>


// Gameplay activity with only the health bar element
class Gameplay : public Activity {
public:
    Background background;
    HpBar bar;

    Gradient gradient;

    // Constructor
    Gameplay() : Activity(),
        background(TFT_BLACK),
        gradient(Element()),
        bar(Element(ivec2(100, 200), LuminaUI_AUTO, ivec2(200, 50)))
    {
        // Add the health bar to the activity's elements list
        Element* elems[] = { &background, &gradient, &bar };
        elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));

    }

    // Render method (calls base Activity render and returns viewport)
    Viewport render(const Viewport &viewport) override {
        Viewport vp = Activity::render(viewport);
        // Additional rendering logic for the health bar can be added here if needed.
        return vp;
    }
};

Gameplay* gameplay = new Gameplay();

#endif // GAMEPLAY_HPP