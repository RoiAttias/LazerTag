#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include "GUI_Manager.hpp"
#include "Utilities/Countdowner.hpp"
#include "Utilities/MoreMath.hpp"

class Activation : public Activity {
public:
    // Elements
    Background background; // A background element
    Text text1;
    Text text2;
    Text text3;
    Text text4;

    // Variables
    uint32_t countdownTime = 500;

    // Constructor
    Activation() : Activity(),
        background(TFT_PURPLE),
        text1(Element(ivec2(0, 30), LuminaUI_AUTO, ivec2(480, 50)), String("Project"), TFT_WHITE, 1, TC_DATUM, 0.0f, &FreeMonoBold18pt7b),
        text2(Element(ivec2(0, 80), LuminaUI_AUTO, ivec2(480, 70)), "LazerTag", TFT_WHITE, 2, TC_DATUM, 0.0f, &FreeMonoBold18pt7b),
        text3(Element(ivec2(0, 180), LuminaUI_AUTO, ivec2(480, 50)), "PRESS ANYWHERE TO PLAY", TFT_WHITE, 1, TC_DATUM, 0.0f, &FreeMono12pt7b),
        text4(Element(ivec2(0, 260), LuminaUI_AUTO, ivec2(480, 50)), "Made by Roi Attias", TFT_WHITE, 1, TC_DATUM, 0.0f, &FreeMonoBold12pt7b)
        {
        // Add all Rectangle elements to the array
        Element* elems[] = {&background, &text1, &text2, &text3, &text4};
        elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));
    }

    Viewport render(const Viewport &viewport) override {
        // Call the base class's render function
        Viewport vp = Activity::render(viewport);
        // Additional rendering logic can be added here if needed.
        return vp;
    }

    void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override {
        if (touchStatus == TouchStatus_PRESS) {
            countdowner->addEvent(countdownTime, GUI::selectActivity, GUI_Manager_Activity::SCANNER);
            Nexus::sendData(COMMS_GAMESTATUS, payloadSizePerCommand[COMMS_GAMESTATUS],
                (uint8_t *)&Game::status, NexusAddress(NEXUS_PROJECT_ID, 0xff, 0xff));
        }
    }
};

Activation *activation = new Activation();

#endif // ACTIVATION_HPP