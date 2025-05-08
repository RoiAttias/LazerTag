/**
 * @file PlayerSetup.hpp
 * @brief Assigns scanned guns & vests to Player 1 and Player 2.
 *
 * Displays two columns (“Player 1”, “Player 2”) each with:
 *  - DeviceBox for gun
 *  - DeviceBox for vest
 *  - Switch buttons to swap assignments
 *  
 * Back/Next navigation at bottom.
 * On Next:
 *  - Sends GAMESTATUS = GAME_STARTING to all
 *  - Advances to “ReadySetGo” Activity
 */

 #ifndef PLAYERSETUP_HPP
 #define PLAYERSETUP_HPP
 
 #include "GUI_Manager.hpp"
 #include "Common/Constants_Common.h"
 #include "Components/Nexus/Nexus.hpp"
 #include "Common/LazerTagPacket.hpp"
 #include "Scanner.hpp"
 
 // Forward declarations
 void onGunSwitchButtonTouch(ivec2, TouchStatus);
 void onVestSwitchButtonTouch(ivec2, TouchStatus);
 void onPlayerSetupBackButtonTouch(ivec2, TouchStatus);
 void onPlayerSetupNextButtonTouch(ivec2, TouchStatus);
 void onPlayer1TitleTouch(ivec2, TouchStatus);
 void onPlayer2TitleTouch(ivec2, TouchStatus);

 // A function from Scanner.hpp
 void triggerScanner(); ///< Forward declaration for scanner trigger function
 
 class PlayerSetup : public Activity {
 public:
     Gradient background;    ///< Gradient fill background
     Text     titleText;     ///< “Player Setup” header
 
     // Player 1 widgets
     Text      player1Title;
     DeviceBox gunBox1, vestBox1;
 
     // Player 2 widgets
     Text      player2Title;
     DeviceBox gunBox2, vestBox2;
 
     Button gunSwitchButton; ///< Swap guns between players
     Button vestSwitchButton;///< Swap vests between players
 
     Button backButton;      ///< Return to Scanner
     Button nextButton;      ///< Advance to ReadySetGo
 
     PlayerSetup()
       : Activity(),
         background(),
         titleText(
           Element(ivec2(0,10), LuminaUI_AUTO, ivec2(480,50)),
           "Player Setup", TFT_WHITE, 1, MC_DATUM, 0, &FreeMonoBold24pt7b
         ),
         player1Title(
           Element(ivec2(0,60), LuminaUI_AUTO, ivec2(240,40)),
           "Player 1", TFT_WHITE, 1, MC_DATUM, 0, &FreeMonoBold18pt7b
         ),
         gunBox1(Element(ivec2(30,110), LuminaUI_AUTO, ivec2(150,50)), 0),
         vestBox1(Element(ivec2(30,170), LuminaUI_AUTO, ivec2(150,50)), 0),
         player2Title(
           Element(ivec2(240,60), LuminaUI_AUTO, ivec2(240,40)),
           "Player 2", TFT_WHITE, 1, MC_DATUM, 0, &FreeMonoBold18pt7b
         ),
         gunBox2(Element(ivec2(300,110), LuminaUI_AUTO, ivec2(150,50)), 0),
         vestBox2(Element(ivec2(300,170), LuminaUI_AUTO, ivec2(150,50)), 0),
         gunSwitchButton(
           Element(ivec2(210,120), LuminaUI_AUTO, ivec2(60,40)),
           "<->", TFT_BLACK, TFT_YELLOW, TFT_BLACK, 10,1,0.0f,
           &FreeMonoBold12pt7b, true, true
         ),
         vestSwitchButton(
           Element(ivec2(210,170), LuminaUI_AUTO, ivec2(60,40)),
           "<->", TFT_BLACK, TFT_YELLOW, TFT_BLACK, 10,1,0.0f,
           &FreeMonoBold12pt7b, true, true
         ),
         backButton(
           Element(ivec2(20,240), LuminaUI_AUTO, ivec2(200,70)),
           "Back", TFT_BLACK, TFT_CYAN, TFT_BLACK, 20,1,0.0f,
           &FreeMono24pt7b, true, true
         ),
         nextButton(
           Element(ivec2(250,240), LuminaUI_AUTO, ivec2(200,70)),
           "Next", TFT_BLACK, TFT_DARKGREY, TFT_BLACK, 20,1,0.0f,
           &FreeMono24pt7b, true, true
         )
     {
         // Configure switch & nav & title buttons
         gunSwitchButton.OnTouch_setHandler(onGunSwitchButtonTouch);
         gunSwitchButton.OnTouch_setEnable(true);
         vestSwitchButton.OnTouch_setHandler(onVestSwitchButtonTouch);
         vestSwitchButton.OnTouch_setEnable(true);
 
         backButton.OnTouch_setHandler(onPlayerSetupBackButtonTouch);
         backButton.OnTouch_setEnable(true);
         nextButton.OnTouch_setHandler(onPlayerSetupNextButtonTouch);
         nextButton.OnTouch_setEnable(true);

         player1Title.OnTouch_setHandler(onPlayer1TitleTouch);
         player1Title.OnTouch_setEnable(true);
         player2Title.OnTouch_setHandler(onPlayer2TitleTouch);
         player2Title.OnTouch_setEnable(true);
 
         // Add all children
         Element* elems[] = {
             &background, &titleText,
             &player1Title, &gunBox1, &vestBox1,
             &player2Title, &gunBox2, &vestBox2,
             &gunSwitchButton, &vestSwitchButton,
             &backButton, &nextButton
         };
         elements.addFromArray(elems, sizeof(elems)/sizeof(Element*));
     }
 
     /**
      * @brief Populate player device addresses from GUI::gameDevices,
      *        then update Next button color.
      */
     void init() {
         Game::reset();
         int g=0, v=0;
         NexusAddress addr;
         
         Game::player1.clearGun();
         Game::player1.clearVest();
            Game::player2.clearGun();
            Game::player2.clearVest();

         for (int i = 0; i < GUI::gameDevices.size(); i++) {
            addr = GUI::gameDevices[i];
             if (addr.groups == NEXUS_GROUP_GUN) {
                if (g == 0) {
                    Game::player1.setGunAddress(addr);
                    g++;
                } else {
                    Game::player2.setGunAddress(addr);
                    g++;
                }
             }
             else if (addr.groups == NEXUS_GROUP_VEST) {
                if (v == 0) {
                    Game::player1.setVestAddress(addr);
                    v++;
                } else {
                    Game::player2.setVestAddress(addr);
                    v++;
                }
             }
         }
         // Enable Next only if both players have both devices
         bool ok = canNext();
         nextButton.background.fillColor = ok ? TFT_ORANGE : TFT_DARKGREY;
         nextButton.background.borderColor = TFT_BLACK;
         nextButton.text.textColor = TFT_BLACK;
     }
 
     /**
      * @brief Check whether both players have gun+vest assigned.
      */
     bool canNext() {
         return Game::canStart();
     }
 
     /**
      * @brief Pull current addresses into DeviceBoxes and show/hide them.
      */
     void update() {
         gunBox1.updateInformation(Game::player1.getGunAddress().deviceID,
                                   Game::player1.getGunAddress().groups);
         gunBox2.updateInformation(Game::player2.getGunAddress().deviceID,
                                   Game::player2.getGunAddress().groups);
         vestBox1.updateInformation(Game::player1.getVestAddress().deviceID,
                                    Game::player1.getVestAddress().groups);
         vestBox2.updateInformation(Game::player2.getVestAddress().deviceID,
                                    Game::player2.getVestAddress().groups);
         gunBox1.visible  = gunBox1.deviceGroup  != 0;
         gunBox2.visible  = gunBox2.deviceGroup  != 0;
         vestBox1.visible = vestBox1.deviceGroup != 0;
         vestBox2.visible = vestBox2.deviceGroup != 0;
     }
 
     Viewport render(const Viewport &vp) override {
         update();
         return Activity::render(vp);
     }
 
     /**
      * @brief Swap the two players’ guns.
      */
     void switchGuns() {
         auto t = Game::player1.getGunAddress();
         Game::player1.setGunAddress(Game::player2.getGunAddress());
         Game::player2.setGunAddress(t);
     }
 
     /**
      * @brief Swap the two players’ vests.
      */
     void switchVests() {
         auto t = Game::player1.getVestAddress();
         Game::player1.setVestAddress(Game::player2.getVestAddress());
         Game::player2.setVestAddress(t);
     }
 };
 
 static PlayerSetup *playerSetup = new PlayerSetup();
 
 // Touch handlers:
    /**
    * @brief Touch event handler for the Gun switch button.
    */
 void onGunSwitchButtonTouch(ivec2, TouchStatus status) {
     if (status == TouchStatus_PRESS) playerSetup->switchGuns();
 }

    /**
     * @brief Touch event handler for the Vest switch button.
     */
 void onVestSwitchButtonTouch(ivec2, TouchStatus status) {
     if (status == TouchStatus_PRESS) playerSetup->switchVests();
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
            triggerScanner();
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
                GUI::selectActivity(GUI_Manager_Activity::PLAYER1GUNSETUP);
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

/**
 * @brief Touch event handler for the Player 1 title.
 */
void onPlayer1TitleTouch(ivec2, TouchStatus status) {
    if (status == TouchStatus_PRESS) {
        if (Game::player1.hasGun()) {
            Nexus::sendData(COMMS_MARK, 0, nullptr,
                        Game::player1.getGunAddress());
        }
        if (Game::player1.hasVest()) {
            Nexus::sendData(COMMS_MARK, 0, nullptr,
                        Game::player1.getVestAddress());
        }
    }
}

/**
 * @brief Touch event handler for the Player 2 title.
 */
void onPlayer2TitleTouch(ivec2, TouchStatus status) {
    if (status == TouchStatus_PRESS) {
        if (Game::player2.hasGun()) {
            Nexus::sendData(COMMS_MARK, 0, nullptr,
                        Game::player2.getGunAddress());
        }
        if (Game::player2.hasVest()) {
            Nexus::sendData(COMMS_MARK, 0, nullptr,
                        Game::player2.getVestAddress());
        }
    }
}
 
 #endif // PLAYERSETUP_HPP 