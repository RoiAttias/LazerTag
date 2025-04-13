#ifndef PLAYERSETUP_HPP
#define PLAYERSETUP_HPP

#include "GUI_Manager.hpp"
#include "Common/Constants_Common.h"
#include <Arduino.h>

#include "Components/Nexus/Nexus.hpp"
#include "Common/LazerTagPacket.hpp"

// Forward declarations for touch event handlers
void onGunSwitchButtonTouch(ivec2 point, TouchStatus status);
void onVestSwitchButtonTouch(ivec2 point, TouchStatus status);
void onPlayerSetupBackButtonTouch(ivec2 point, TouchStatus status);
void onPlayerSetupNextButtonTouch(ivec2 point, TouchStatus status);

class PlayerSetup : public Activity {
public:
    // Background and Title
    Gradient background;
    Text titleText;

    // Player 1 elements
    Text player1Title;
    DeviceBox gunBox1;
    DeviceBox vestBox1;

    // Player 2 elements
    Text player2Title;
    DeviceBox gunBox2;
    DeviceBox vestBox2;

    // Switch buttons for guns and vests
    Button gunSwitchButton;
    Button vestSwitchButton;

    // Navigation buttons
    Button backButton;
    Button nextButton;

    /**
     * @brief Construct a PlayerSetup activity.
     */
    PlayerSetup() : Activity(),
        background(),
        titleText(Element(ivec2(0, 10), LuminaUI_AUTO, ivec2(480, 50)),
                  String("Player Setup"), TFT_WHITE, 1, MC_DATUM, 0, &FreeMonoBold24pt7b),

        player1Title(Element(ivec2(0, 60), LuminaUI_AUTO, ivec2(240, 40)),
                     String("Player 1"), TFT_WHITE, 1, MC_DATUM, 0, &FreeMonoBold18pt7b),
        gunBox1(Element(ivec2(30, 110), LuminaUI_AUTO, ivec2(150, 50)), 0),
        vestBox1(Element(ivec2(30, 170), LuminaUI_AUTO, ivec2(150, 50)), 0),

        player2Title(Element(ivec2(240, 60), LuminaUI_AUTO, ivec2(240, 40)),
                     String("Player 2"), TFT_WHITE, 1, MC_DATUM, 0, &FreeMonoBold18pt7b),
        gunBox2(Element(ivec2(300, 110), LuminaUI_AUTO, ivec2(150, 50)), 0),
        vestBox2(Element(ivec2(300, 170), LuminaUI_AUTO, ivec2(150, 50)), 0),

        gunSwitchButton(Element(ivec2(210, 120), LuminaUI_AUTO, ivec2(60, 40)),
                        "<->", TFT_BLACK, TFT_YELLOW, TFT_BLACK, 10, 1, 0.0f,
                        &FreeMonoBold12pt7b, true, true),
        vestSwitchButton(Element(ivec2(210, 170), LuminaUI_AUTO, ivec2(60, 40)),
                         "<->", TFT_BLACK, TFT_YELLOW, TFT_BLACK, 10, 1, 0.0f,
                         &FreeMonoBold12pt7b, true, true),

        backButton(Element(ivec2(20, 240), LuminaUI_AUTO, ivec2(200, 70)),
                   "Back", TFT_BLACK, TFT_CYAN, TFT_BLACK, 20, 1, 0.0f,
                   &FreeMono24pt7b, true, true),
        nextButton(Element(ivec2(250, 240), LuminaUI_AUTO, ivec2(200, 70)),
                   "Next", TFT_BLACK, TFT_DARKGREY, TFT_BLACK, 20, 1, 0.0f,
                   &FreeMono24pt7b, true, true)
    {
        // Set touch event handlers for the switch buttons
        gunSwitchButton.OnTouch_setHandler(onGunSwitchButtonTouch);
        gunSwitchButton.OnTouch_setEnable(true);
        
        vestSwitchButton.OnTouch_setHandler(onVestSwitchButtonTouch);
        vestSwitchButton.OnTouch_setEnable(true);

        // Set touch event handlers for navigation buttons
        backButton.OnTouch_setHandler(onPlayerSetupBackButtonTouch);
        backButton.OnTouch_setEnable(true);
        
        nextButton.OnTouch_setHandler(onPlayerSetupNextButtonTouch);
        nextButton.OnTouch_setEnable(true);

        // Add all elements to the activity.
        Element* elems[12] = {
            &background,
            &titleText,
            &player1Title,
            &gunBox1,
            &vestBox1,
            &player2Title,
            &gunBox2,
            &vestBox2,
            &gunSwitchButton,
            &vestSwitchButton,
            &backButton,
            &nextButton
        };
        elements.addFromArray(elems, sizeof(elems)/sizeof(Element*));
    }

    void init() {
        for (int i = 0, g = 0, v = 0; i < GUI::gameDevices.size(); i++) {
            if (GUI::gameDevices[i].groups == NEXUS_GROUP_GUN) {
                if (g == 0) {
                    Game::player1.setGunAddress(GUI::gameDevices[i]);
                } else if (g == 1) {
                    Game::player2.setGunAddress(GUI::gameDevices[i]);
                }
                g++;
            } else if (GUI::gameDevices[i].groups == NEXUS_GROUP_VEST) {
                if (v == 0) {
                    Game::player1.setVestAddress(GUI::gameDevices[i]);
                } else if (v == 1) {
                    Game::player2.setVestAddress(GUI::gameDevices[i]);
                }
                v++;
            }
        }

        // Set the initial state of the next button
        if (canNext())
        {
            nextButton.background.fillColor = TFT_ORANGE;
            nextButton.background.borderColor = TFT_BLACK;
            nextButton.text.textColor = TFT_BLACK;
        } else {
            nextButton.background.fillColor = TFT_DARKGREY;
            nextButton.background.borderColor = TFT_BLACK;
            nextButton.text.textColor = TFT_BLACK;
        }
    }

    /**
     * @brief Check if the next button can be pressed.
     * @return true if the next button can be pressed, false otherwise.
     */
    bool canNext() {
        return Game::canStart();
    }

    void update() {
        // Update the device boxes with the current game devices.
        gunBox1.updateInformation(Game::player1.getGunAddress().deviceID, Game::player1.getGunAddress().groups);
        gunBox2.updateInformation(Game::player2.getGunAddress().deviceID, Game::player2.getGunAddress().groups);
        vestBox1.updateInformation(Game::player1.getVestAddress().deviceID, Game::player1.getVestAddress().groups);
        vestBox2.updateInformation(Game::player2.getVestAddress().deviceID, Game::player2.getVestAddress().groups);

         // Update the visibility of the device boxes based on their group.
         gunBox1.visible = (gunBox1.deviceGroup != 0);
         gunBox2.visible = (gunBox2.deviceGroup != 0);
         vestBox1.visible = (vestBox1.deviceGroup != 0);
         vestBox2.visible = (vestBox2.deviceGroup != 0);
    }

    void switchGuns() {
        NexusAddress temp = Game::player1.getGunAddress();
        Game::player1.setGunAddress(Game::player2.getGunAddress());
        Game::player2.setGunAddress(temp);
    }

    void switchVests() {
        NexusAddress temp = Game::player1.getVestAddress();
        Game::player1.setVestAddress(Game::player2.getVestAddress());
        Game::player2.setVestAddress(temp);
    }

    /**
     * @brief Override the render method if additional custom rendering is needed.
     *
     * @param viewport The viewport to render in.
     * @return The clamped viewport.
     */
    virtual Viewport render(const Viewport &viewport) override {
        update(); // Update the device boxes with the current game devices.

        // Call the base class render method
        return Activity::render(viewport);
    }
};

PlayerSetup *playerSetup = new PlayerSetup();


/**
 * @brief Touch event handler for the Gun Switch button.
 */
void onGunSwitchButtonTouch(ivec2 point, TouchStatus status) {
    if (status == TouchStatus::TouchStatus_PRESS) {
        playerSetup->switchGuns();
    }
}

/**
 * @brief Touch event handler for the Vest Switch button.
 */
void onVestSwitchButtonTouch(ivec2 point, TouchStatus status) {
    if (status == TouchStatus::TouchStatus_PRESS) {
        playerSetup->switchVests();
    }
}

/**
 * @brief Touch event handler for the Back button.
 */
void onPlayerSetupBackButtonTouch(ivec2 point, TouchStatus status) {
    switch (status) {
        case TouchStatus::TouchStatus_RELEASE:
            playerSetup->backButton.background.fillColor = TFT_CYAN;
            playerSetup->backButton.background.borderColor = TFT_BLACK;
            playerSetup->backButton.text.textColor = TFT_BLACK;
            playerSetup->backButton.callRender();

            // Go back to the previous activity
            GUI::selectActivity(GUI_Manager_Activity::SCANNER);
            break;

        case TouchStatus::TouchStatus_PRESS:
            // Change the button color when pressed
            playerSetup->backButton.background.fillColor = TFT_BLUE;
            playerSetup->backButton.background.borderColor = TFT_WHITE;
            playerSetup->backButton.text.textColor = TFT_WHITE;
            playerSetup->backButton.callRender();
            break;

        case TouchStatus::TouchStatus_READY:
            // Change the button color back when released
            playerSetup->backButton.background.fillColor = TFT_CYAN;
            playerSetup->backButton.background.borderColor = TFT_BLACK;
            playerSetup->backButton.text.textColor = TFT_BLACK;
            playerSetup->backButton.callRender();
            break;

        default:
            break;
    }
}

/**
 * @brief Touch event handler for the Next button.
 */
void onPlayerSetupNextButtonTouch(ivec2 point, TouchStatus status) {
    if (playerSetup->canNext()) {
        switch (status) {
            case TouchStatus::TouchStatus_RELEASE:
                playerSetup->nextButton.background.fillColor = TFT_ORANGE;
                playerSetup->nextButton.background.borderColor = TFT_BLACK;
                playerSetup->nextButton.text.textColor = TFT_BLACK;
                playerSetup->nextButton.callRender();

                // Proceed to the next step
                GUI::selectActivity(GUI_Manager_Activity::READYSETGO);
                Game::start();
                Nexus::sendData(COMMS_GAMESTATUS, payloadSizePerCommand[COMMS_GAMESTATUS],
                    (uint8_t *)&Game::status, NexusAddress(NEXUS_PROJECT_ID, 0xff, 0xff));
                break;

            case TouchStatus::TouchStatus_PRESS:
                // Change the button color when pressed
                playerSetup->nextButton.background.fillColor = TFT_MAROON;
                playerSetup->nextButton.background.borderColor = TFT_YELLOW;
                playerSetup->nextButton.text.textColor = TFT_YELLOW;
                playerSetup->nextButton.callRender();
                break;

            case TouchStatus::TouchStatus_READY:
                // Change the button color back when released
                playerSetup->nextButton.background.fillColor = TFT_ORANGE;
                playerSetup->nextButton.background.borderColor = TFT_BLACK;
                playerSetup->nextButton.text.textColor = TFT_BLACK;
                playerSetup->nextButton.callRender();
                break;
             
            default:
                break;
        }
    } else {
        // Change the button color to indicate it cannot be pressed
        playerSetup->nextButton.background.fillColor = TFT_DARKGREY;
        playerSetup->nextButton.background.borderColor = TFT_BLACK;
        playerSetup->nextButton.text.textColor = TFT_BLACK;
        playerSetup->nextButton.callRender();
    }
}

#endif // PLAYERSETUP_HPP
