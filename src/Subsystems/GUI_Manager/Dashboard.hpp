#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include "GUI_Manager.hpp"
#include <Arduino.h>

class Dashboard : public Activity {
public:
    // Elements
    Rectangle header;         // A header rectangle for the title
    Text titleText;           // Title text displayed in the header
    Ellipse statusIndicator;   // A circle indicating system status
    Rectangle buttonRect;     // A button rectangle

    // Constructor
    Dashboard() : Activity(),
        header(Element(ivec2(0, 0), TGUI_AUTO, ivec2(240, 40)), TFT_BLUE, TFT_BLACK),
        titleText(Element(ivec2(120, 20), TGUI_AUTO, TGUI_AUTO), "Dashboard", TFT_WHITE, 1, 1, 2.5f, &FreeSans12pt7b),
        statusIndicator(Element(ivec2(50, 100), TGUI_AUTO,ivec2(50, 30)), TFT_GREEN, TFT_BLACK, true, true),
        buttonRect(Element(ivec2(70, 260), TGUI_AUTO, ivec2(100, 40)), TFT_RED, TFT_BLACK, true, true, 10)
    {
        // Align title text to the center of the header
        titleText.origin = ivec2(header.offset.x + header.scale.x / 2, header.offset.y + header.scale.y / 2);

        // Add all elements to the `elements` HyperList
        Element* elems[] = {&header, &titleText, &statusIndicator, &buttonRect};
        elements.addFromArray(elems, 4);
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
