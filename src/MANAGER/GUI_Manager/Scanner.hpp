#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "GUI_Manager.hpp"
#include "Common/Constants_Common.h"
#include <Arduino.h>

#include "Components/Nexus/Nexus.hpp"
#include "Common/LazerTagPacket.hpp"

#include "PlayerSetup.hpp"

void onScanButtonTouch(ivec2 point, TouchStatus status);
void onNextButtonTouch(ivec2 point, TouchStatus status);
void onDeviceBoxTouch(ivec2 point, TouchStatus status);

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
        nextButton(Element(ivec2(250, 240), LuminaUI_AUTO, ivec2(200, 70)), "Next", TFT_BLACK, TFT_DARKGREY, TFT_BLACK, 20, 1, 0.0f,
            &FreeMono24pt7b, true, true)
    {
        // Set the touch event handler for the scan button
        scanButton.OnTouch_setHandler(onScanButtonTouch);
        scanButton.OnTouch_setEnable(true);

        // Set the touch event handler for the next button
        nextButton.OnTouch_setHandler(onNextButtonTouch);
        nextButton.OnTouch_setEnable(true);

        // Add static elements to the activity
        Element* elems[13] = {&background, &titleText, &scanButton, &nextButton};

        // Create 9 DeviceBoxes with a default size (150x50).
        // Device IDs will be 1 through 9.
        ivec2 deviceBoxPos;
        for (int i = 0; i < 9; i++) {
            deviceBoxPos = deviceBoxOrigin + (deviceBoxSize + deviceBoxSpacing).multiply(ivec2(i % 3, i / 3));
            deviceBoxes[i] = new DeviceBox(Element(deviceBoxPos, LuminaUI_AUTO, ivec2(150, 50)), i + 1);
            elems[4 + i] = deviceBoxes[i]; // Add the DeviceBox to the elements list
        }

        init(); // Initialize the DeviceBoxes

        // Add all elements to the activity
        elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));
    }

    void init() {
        // Initialize the DeviceBoxes
        for (int i = 0; i < 9; i++) {
            deviceBoxes[i]->visible = false; // Initially hide the DeviceBoxes
            deviceBoxes[i]->OnTouch_setHandler(onDeviceBoxTouch); // Set the touch event handler
            deviceBoxes[i]->OnTouch_setEnable(true); // Enable touch events
        }
    }

    /**
     * @brief Check if the next button can be pressed.
     * 
     * @return true if the next button can be pressed, false otherwise.
     */
    bool canNext() {
        int countGuns = 0;
        int countVests = 0;

        for (int i = 0; i < 9; i++) {
            if (deviceBoxes[i]->selected) {
                if (deviceBoxes[i]->deviceGroup == NEXUS_GROUP_GUN) {
                    countGuns++;
                } else if (deviceBoxes[i]->deviceGroup == NEXUS_GROUP_VEST) {
                    countVests++;
                }
            }
        }

        return (countGuns > 0 && countVests > 0);
    }

    /**
     * @brief Override the render method if additional custom rendering is needed.
     *
     * @param viewport The viewport to render in.
     * @return The clamped viewport.
     */
    virtual Viewport render(const Viewport &viewport) override {
        GUI::gameDevices.clear(); // Clear the game devices list
        DeviceBox* deviceBox;
        for (int i = 0; i < 9; i++) {
            deviceBox = deviceBoxes[i];
            if (deviceBox->selected) {
                uint8_t group = deviceBox->deviceGroup;
                if (group == NEXUS_GROUP_GUN || group == NEXUS_GROUP_VEST) {
                    // Send a scan request to the selected device
                    NexusAddress deviceAddress = {NEXUS_PROJECT_ID, group, (uint8_t)deviceBox->deviceId};
                    GUI::gameDevices.addend(deviceAddress);
                }
            } else {
                break; // Exit the loop if the device is not a gun or vest
            }
        }

        // Call the base class render method
        Viewport vp = Activity::render(viewport);
        // Additional custom rendering code for Scanner can be added here if needed.
        return vp;
    }

    virtual void updateScannedDevices() {
        // Update the DeviceBoxes with the scanned devices.
        for (int i = 0; i < 9; i++) {
            if (i < Nexus::devices.size()) {
                deviceBoxes[i]->updateInformation(Nexus::devices[i].deviceID, Nexus::devices[i].groups);
                deviceBoxes[i]->setSelected(false);
                deviceBoxes[i]->visible = true;
            } else {
                deviceBoxes[i]->updateInformation(0, 0);
                deviceBoxes[i]->setSelected(false);
                deviceBoxes[i]->visible = false;
            }
        }

        scanButton.background.fillColor = TFT_GREEN;
        nextButton.background.fillColor = canNext() ? TFT_ORANGE : TFT_DARKGREY;
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
            scanner->scanButton.background.fillColor = TFT_DARKGREY;
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
            scanner->scanButton.background.fillColor = Nexus::isScanComplete ? TFT_GREEN : TFT_DARKGREY;
            scanner->scanButton.background.borderColor = TFT_BLACK;
            scanner->scanButton.text.textColor = TFT_BLACK;
            scanner->scanButton.callRender();
            break;

        default:
            break;
    }
}

/**
 * @brief Touch event handler for the next button.
 */
void onNextButtonTouch(ivec2 point, TouchStatus status) {
    if (scanner->canNext()) {
        switch (status) {
            case TouchStatus::TouchStatus_RELEASE:
                scanner->nextButton.background.fillColor = TFT_ORANGE;
                scanner->nextButton.background.borderColor = TFT_BLACK;
                scanner->nextButton.text.textColor = TFT_BLACK;
                scanner->nextButton.callRender();

                // Proceed to the next step
                GUI::selectActivity(GUI_Manager_Activity::PLAYERSETUP);
                playerSetup->init();
                break;

            case TouchStatus::TouchStatus_PRESS:
                // Change the button color when pressed
                scanner->nextButton.background.fillColor = TFT_MAROON;
                scanner->nextButton.background.borderColor = TFT_YELLOW;
                scanner->nextButton.text.textColor = TFT_YELLOW;
                scanner->nextButton.callRender();
                break;

            case TouchStatus::TouchStatus_READY:
                // Change the button color back when released
                scanner->nextButton.background.fillColor = TFT_ORANGE;
                scanner->nextButton.background.borderColor = TFT_BLACK;
                scanner->nextButton.text.textColor = TFT_BLACK;
                scanner->nextButton.callRender();
                break;

             
            default:
                break;
        }
    } else {
        // Change the button color to indicate it cannot be pressed
        scanner->nextButton.background.fillColor = TFT_DARKGREY;
        scanner->nextButton.background.borderColor = TFT_BLACK;
        scanner->nextButton.text.textColor = TFT_BLACK;
        scanner->nextButton.callRender();
    }
}

void onDeviceBoxTouch(ivec2 point, TouchStatus status) {
    DeviceBox* deviceBox;
    for (int i = 0; i < 9; i++) {
        deviceBox = scanner->deviceBoxes[i];
        if (deviceBox->inRange(point) && deviceBox->visible && status == TouchStatus::TouchStatus_PRESS) {
            // Handle the touch event for the DeviceBox
            deviceBox->invertSelected();

            if (deviceBox->selected) {
                uint8_t group = deviceBox->deviceGroup;
                if (group == NEXUS_GROUP_GUN || group == NEXUS_GROUP_VEST) {
                    // Send a scan request to the selected device
                    NexusAddress deviceAddress = {NEXUS_PROJECT_ID, group, (uint8_t)deviceBox->deviceId};
                    Nexus::sendData(COMMS_MARK, 0, nullptr, deviceAddress);
                } else {
                    // Deselect the device
                    deviceBox->setSelected(false);
                }
            }
            break;
        }
    }
    GUI::callRender();
}

#endif // SCANNER_HPP
