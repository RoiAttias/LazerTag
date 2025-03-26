#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "GUI_Manager.hpp"
#include "Common/Constants_Common.h"
#include <Arduino.h>

void onScanButtonTouch(ivec2 point, TouchStatus status);

class Scanner : public Activity {
public:
    // Elements
    Background background;     // A background element
    Text titleText;            // Title text displayed in the header
    Button scanButton;         // A scan button
    Button nextButton;         // A next button

    DeviceBox* deviceBoxes[9]; // Pointers to DeviceBoxes for devices 1-9

    ivec2 deviceBoxOrigin = ivec2(6, 60); // Default origin for DeviceBoxes
    ivec2 deviceBoxSize = ivec2(150, 50); // Default size for DeviceBoxes
    ivec2 deviceBoxSpacing = ivec2(6, 8); // Default spacing between DeviceBoxes

    /**
     * @brief Construct a Scanner activity.
     */
    Scanner() : Activity(),
        background(TFT_BROWN),
        titleText(Element(ivec2(0, 10), LuminaUI_AUTO, ivec2(480, 40)), String("Scanner"), TFT_WHITE, 1, MC_DATUM, 0, &FreeMonoBold24pt7b),
        scanButton(Element(ivec2(20, 240), LuminaUI_AUTO, ivec2(200, 70)), "Scan", TFT_BLACK, TFT_GREEN, TFT_BLACK, 20, 1, 0.0f,
            &FreeMono24pt7b, true, true),
        nextButton(Element(ivec2(250, 240), LuminaUI_AUTO, ivec2(200, 70)), "Next", TFT_BLACK, TFT_ORANGE, TFT_BLACK, 20, 1, 0.0f,
            &FreeMono24pt7b, true, true)
    {
        // Set the touch event handler for the scan button
        scanButton.OnTouch_setHandler(onScanButtonTouch);
        scanButton.OnTouch_setEnable(true);

        // Add static elements to the activity
        Element* elems[13] = {&background, &titleText, &scanButton, &nextButton};

        // Create 9 DeviceBoxes with a default size (150x50).
        // Device IDs will be 1 through 9.
        ivec2 deviceBoxPos;
        for (int i = 0; i < 9; i++) {
            deviceBoxPos = deviceBoxOrigin + (deviceBoxSize + deviceBoxSpacing).multiply(ivec2(i % 3, i / 3));
            deviceBoxes[i] = new DeviceBox(Element(deviceBoxPos, LuminaUI_AUTO, ivec2(150, 50)), i + 1);
            elems[4 + i] = deviceBoxes[i];
            elems[4 + i]->visible = false;
        }

        elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));
    }

    /**
     * @brief Override the render method if additional custom rendering is needed.
     *
     * @param viewport The viewport to render in.
     * @return The clamped viewport.
     */
    virtual Viewport render(const Viewport &viewport) override {
        Viewport vp = Activity::render(viewport);
        // Additional custom rendering code for Scanner can be added here if needed.
        return vp;
    }

    virtual void setScannedDevices(HyperList<NexusAddress> devices) {
        // Update the DeviceBoxes with the scanned devices.
        for (int i = 0; i < 9; i++) {
            if (i < devices.size()) {
                deviceBoxes[i]->updateInformation(devices[i].deviceID, deviceGroupString(devices[i].groups));
                deviceBoxes[i]->visible = true;
            } else {
                deviceBoxes[i]->updateInformation(0, "NEW");
                deviceBoxes[i]->visible = false;
            }
        }

        // Call the render method to update the display.
        callRender();
    }
};

Scanner *scanner = new Scanner();


    /**
     * @brief Touch event handler for the scan button.
     */
    void onScanButtonTouch(ivec2 point, TouchStatus status) {
        switch (status) {
            case TouchStatus::TouchStatus_RELEASE:
                // Start scanning for devices
                Nexus::scan();
                scanner->scanButton.background.fillColor = TFT_GREEN;
                scanner->scanButton.background.borderColor = TFT_BLACK;
                scanner->scanButton.text.textColor = TFT_BLACK;
                scanner->scanButton.callRender();
                break;

            case TouchStatus::TouchStatus_PRESS:
                // Change the button color when pressed
                scanner->scanButton.background.fillColor = TFT_DARKGREEN;
                scanner->scanButton.background.borderColor = TFT_WHITE;
                scanner->scanButton.text.textColor = TFT_WHITE;
                scanner->scanButton.callRender();
                break;
            
            case TouchStatus::TouchStatus_READY:
                // Change the button color back when released
                scanner->scanButton.background.fillColor = TFT_GREEN;
                scanner->scanButton.background.borderColor = TFT_BLACK;
                scanner->scanButton.text.textColor = TFT_BLACK;
                scanner->scanButton.callRender();
                break;

            default:
                break;
        }
    }

#endif // SCANNER_HPP
