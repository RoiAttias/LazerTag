#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include "GUI_Manager.hpp"
#include "Utilities/Countdowner.hpp"
#include "Utilities/MoreMath.hpp"

void activationTouch(int param);

class Activation : public Activity {
public:
    // Elements
    Background background; // A background element
    Text text1;
    Text text2;
    Text text3;
    Text text4;

    // Timing
    uint32_t countdownTime = 500;

    // Other
    ivec2 textScale = ivec2(480,40);

    // Constructor
    Activation() : Activity(),
        background(TFT_PURPLE),
        text1(Element(ivec2(0, 30), LuminaUI_AUTO, textScale, true, true), "Project", TFT_WHITE, 1, TC_DATUM, 0.0f, &FreeMonoBold18pt7b),
        text2(Element(ivec2(0, 80), LuminaUI_AUTO, textScale, true, true), "LazerTag", TFT_WHITE, 2, TC_DATUM, 0.0f, &FreeMonoBold18pt7b),
        text3(Element(ivec2(0, 180), LuminaUI_AUTO, textScale, true, true), "PRESS ANYWHERE TO PLAY", TFT_WHITE, 1, TC_DATUM, 0.0f, &FreeMono12pt7b),
        text4(Element(ivec2(0, 260), LuminaUI_AUTO, textScale, true, true), "Made by Roi Attias", TFT_WHITE, 1, TC_DATUM, 0.0f, &FreeMonoBold12pt7b)
    {
        // Activity settings
        OnTouch_setEnable(false);
        renderAlways = false;

        // Add all Rectangle elements to the array
        Element* elems[] = {&background, &text1, &text2, &text3, &text4};
        elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));

        countdowner->addEvent(countdownTime, activationTouch, 0);
    }

    void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override {
        if (touchStatus == TouchStatus_PRESS) {
            countdowner->addEvent(countdownTime, GUI::selectActivity, GUI_Manager_Activity::SCANNER);
        }
    }
};

Activation *activation = new Activation();

void activationTouch(int param) {
    activation->OnTouch_setEnable(true);
}

#endif // ACTIVATION_HPP
