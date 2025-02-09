#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include "GUI_Manager.hpp"
#include <Arduino.h>

class Dashboard : public Activity {
public:
    // Elements
    Background background;     // A background element
    Rectangle header;         // A header rectangle for the title
    Text titleText;           // Title text displayed in the header
    Ellipse statusIndicator;   // A circle indicating system status
    Rectangle buttonRect;     // A button rectangle

    // Constructor
    Dashboard() : Activity(),
        background(TFT_BROWN),
        header(Element(ivec2(0, 0), LuminaUI_AUTO, ivec2(240, 40)), TFT_BLUE, TFT_BLACK),
        titleText(Element(ivec2(120, 20), LuminaUI_AUTO, LuminaUI_AUTO), String("Dashboard"), TFT_WHITE, 1, 1, 2.5f, &FreeSans9pt7b),
        statusIndicator(Element(ivec2(50, 100), LuminaUI_AUTO,ivec2(50, 30)), TFT_GREEN, TFT_BLACK, true, true),
        buttonRect(Element(ivec2(70, 260), LuminaUI_AUTO, ivec2(100, 40)), TFT_RED, TFT_BLACK, true, true, 10)
    {
        // Add all elements to the `elements` HyperList
        Element* elems[] = {&background ,&header, &titleText, &statusIndicator, &buttonRect};
        elements.addFromArray(elems, 5);
    }

    /**
     * @brief Override the onTouch behavior to handle button interaction
     * @param point The point of touch.
     * @param touchStatus The status of the touch.
     */
    virtual void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override {
        Serial.println("Dashboard");
    }
};

#endif // DASHBOARD_HPP
