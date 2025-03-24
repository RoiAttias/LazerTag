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

    Grid grid;                // A grid layout

    DeviceBox deviceBox1;     // DeviceBox for device 1
    DeviceBox deviceBox2;     // DeviceBox for device 2
    DeviceBox deviceBox3;     // DeviceBox for device 3
    DeviceBox deviceBox4;     // DeviceBox for device 4

    // Constructor
    Dashboard() : Activity(),
        background(TFT_BROWN),
        header(Element(ivec2(0, 0), LuminaUI_AUTO, ivec2(240, 40)), TFT_BLUE, TFT_BLACK),
        titleText(Element(ivec2(120, 20), LuminaUI_AUTO, LuminaUI_AUTO), String("Dashboard"), TFT_WHITE, 1, 1, 2.5f, &FreeMono24pt7b),
        statusIndicator(Element(ivec2(50, 100), LuminaUI_AUTO,ivec2(50, 30)), TFT_GREEN, TFT_BLACK, true, true),
        buttonRect(Element(ivec2(70, 260), LuminaUI_AUTO, ivec2(100, 40)), TFT_RED, TFT_BLACK, true, true, 10),
        grid(Element(ivec2(0, 40), LuminaUI_AUTO, ivec2(300, 100))),
        deviceBox1(Element(ivec2(0,40), LuminaUI_AUTO, ivec2(150,50)), 1, "VEST"),
        deviceBox2(Element(ivec2(150,40), LuminaUI_AUTO, ivec2(150,50)), 2, "GUN"),
        deviceBox3(Element(ivec2(0,90), LuminaUI_AUTO, ivec2(150,50)), 3, "Vest"),
        deviceBox4(Element(ivec2(150,90), LuminaUI_AUTO, ivec2(150,50)), 4, "Gun")
    {
        // Add columns and rows to the grid
        grid.addColumn(150);
        grid.addColumn(150);
        grid.addColumn(150);
        grid.addRow(50);
        grid.addRow(50);
        grid.addRow(50);
        /*
        // Add cells to the grid
        grid.addCell(new DeviceBox(Element(LuminaUI_AUTO, LuminaUI_AUTO, ivec2(150,50)), 1, "VEST"), ivec2(0, 0));
        grid.addCell(new DeviceBox(Element(LuminaUI_AUTO, LuminaUI_AUTO, ivec2(150,50)), 2, "GUN"), ivec2(0, 1));
        grid.addCell(new DeviceBox(Element(LuminaUI_AUTO, LuminaUI_AUTO, ivec2(150,50)), 3, "Vest"), ivec2(1, 0));
        grid.addCell(new DeviceBox(Element(LuminaUI_AUTO, LuminaUI_AUTO, ivec2(150,50)), 4, "Gun"), ivec2(1, 1));

        */
        // Add all elements to the `elements` HyperList
        Element* elems[] = {&background ,&header, &titleText, &statusIndicator, &buttonRect, &deviceBox1, &deviceBox2, &deviceBox3, &deviceBox4};
        elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));
    }

    Viewport render(const Viewport &viewport) override {
        Viewport vp =  Activity::render(viewport);

        /*
        // Define vertices for the gun shape
        ivec2 gunVertices[] = {
            {10, 20}, {30, 20}, {35, 25}, {50, 25}, {50, 35}, {30, 35}, {25, 40}, {10, 40}, // Main body
            {10, 50}, {15, 50}, {15, 40}, {10, 40},   // Grip
            {40, 20}, {50, 20}, {50, 25}, {40, 25},   // Barrel extension
        };

        // Define triangles using indices from gunVertices
        uint gunTriangles[] = {
            0, 1, 7,  1, 6, 7,  1, 2, 6,  2, 5, 6,  2, 3, 5,  3, 4, 5,  // Main body
            7, 8, 11, 8, 9, 11, 9, 10, 11,   // Grip
            12, 13, 15, 12, 14, 15           // Barrel extension
        };

        // Call the function to draw the gun
        LuminaUI::fillPolygon(&tft, gunVertices, sizeof(gunVertices) / sizeof(ivec2),
                    gunTriangles, sizeof(gunTriangles) / sizeof(uint), TFT_WHITE);

        LuminaUI::drawTriangles(&tft, gunVertices, sizeof(gunVertices) / sizeof(ivec2),
                    gunTriangles, sizeof(gunTriangles) / sizeof(uint), TFT_BLACK);

        */
        return vp;
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
