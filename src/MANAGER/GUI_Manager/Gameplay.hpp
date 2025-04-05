#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "GUI_Manager.hpp"
#include <Arduino.h>

#include "Utilities/Countdowner.hpp"

void countdownerCallback(int param);

// Gameplay activity with only the health bar element
class Gameplay : public Activity {
public:
    Background background;
    HpBar bar;

    // Constructor
    Gameplay() : Activity(),
        background(TFT_BLACK),
        bar(Element(ivec2(100, 200), LuminaUI_AUTO, ivec2(200, 50)))
    {
        // Add the health bar to the activity's elements list
        Element* elems[] = { &background, &bar };
        elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));

        countdowner->addEvent(1000, countdownerCallback);
    }

    // Render method (calls base Activity render and returns viewport)
    Viewport render(const Viewport &viewport) override {
        Viewport vp = Activity::render(viewport);
        // Additional rendering logic for the health bar can be added here if needed.
        return vp;
    }
};

Gameplay* gameplay = new Gameplay();

int health = 0;
void countdownerCallback(int param) {
    health += 2;
    if (health > 100) {
        health = 0;
    }

    gameplay->bar.setValue(health);
    GUI::callRender();

    countdowner->addEvent(200, countdownerCallback);
}

#endif // GAMEPLAY_HPP