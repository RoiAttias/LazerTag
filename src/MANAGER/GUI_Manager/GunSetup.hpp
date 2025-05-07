 #ifndef GUNSETUP_HPP
 #define GUNSETUP_HPP
 
 #include "GUI_Manager.hpp"
 #include "Components/Nexus/Nexus.hpp"
 #include "Common/LazerTagPacket.hpp"

 
 // Forward declarations
 void onPlayer1PrevGunButtonTouch(ivec2, TouchStatus);
 void onPlayer1NextGunButtonTouch(ivec2, TouchStatus);
 void onPlayer1GunSetupBackButtonTouch(ivec2, TouchStatus);
 void onPlayer1GunSetupNextButtonTouch(ivec2, TouchStatus);

    void onPlayer2PrevGunButtonTouch(ivec2, TouchStatus);
    void onPlayer2NextGunButtonTouch(ivec2, TouchStatus);
    void onPlayer2GunSetupBackButtonTouch(ivec2, TouchStatus);
    void onPlayer2GunSetupNextButtonTouch(ivec2, TouchStatus);

/**
 * @brief Function to start the game and transition to the ReadySetGo activity.
 *
 * - Sets the game status to STARTING.
 * - Sends the game status to all devices.
 * - Calls Game::start() to initialize the game.
 */
void moveToRSG() {
    GUI::selectActivity(GUI_Manager_Activity::READYSETGO);
    Game::start();
    Nexus::sendData(COMMS_GAMESTATUS, payloadSizePerCommand[COMMS_GAMESTATUS],
        (uint8_t *)&Game::status, NexusAddress(NEXUS_PROJECT_ID, 0xff, 0xff));
}
 
 class GunSetup : public Activity {
 public:
     Gradient background;    ///< Gradient fill background
     Text     titleText;     ///< “Player1 - Gun Setup” header
    
     Polygon prevTriangle; ///< Previous gun triangle
     Text gunName;    ///< Gun name text box
     Polygon nextTriangle; ///< Next gun triangle
 
     Text descriptionTitle; ///< Gun description title
     Textbox gunDescription; ///< Gun description text box

     Button backButton;      ///< Return to PlayerSetup or Player1GunSetup
     Button nextButton;      ///< Advance to ReadySetGo or Player2GunSetup

     int playerNumber; ///< Player number
     int gunIndex = 0; ///< Gun index for the current player
 
     GunSetup(int playerNumber = 0)
       : Activity(), playerNumber(playerNumber),
         background(Element(ivec2(0,0), LuminaUI_AUTO, ivec2(480,320)),
            255,135, 30,
            0, 0, 0,
            true),
         titleText(
           Element(ivec2(0,10), LuminaUI_AUTO, ivec2(480,50)),
           "Gun Setup - Player " + playerNumber, TFT_WHITE, 1, MC_DATUM, 0, &FreeMonoBold24pt7b
         ),
        prevTriangle(
            Element(ivec2(20,70), LuminaUI_AUTO, ivec2(40,50)),
            polygon_vertices_example,
            polygon_vertices_example_count,
            polygon_triangles_example,
            polygon_triangles_example_count, 
            TFT_WHITE, TFT_BLACK, true, true,
            vec2(0.0f, 0.0f),vec2(1.0f, 1.0f)
        ),
        gunName(
            Element(ivec2(70,70), LuminaUI_AUTO, ivec2(150,50)),
            String("Gun Name"), TFT_WHITE, 1, TC_DATUM, 1.1f, &FreeMono9pt7b
        ),
        nextTriangle(
            Element(ivec2(230,70), LuminaUI_AUTO, ivec2(40,50)),
            polygon_vertices_example,
            polygon_vertices_example_count,
            polygon_triangles_example,
            polygon_triangles_example_count, 
            TFT_WHITE, TFT_BLACK, true, true,
            vec2(0.0f, 0.0f),vec2(1.0f, 1.0f)
        ),
        descriptionTitle(
            Element(ivec2(280,70), LuminaUI_AUTO, ivec2(150,30)),
            "Description:", TFT_WHITE, 1, TC_DATUM, 0, &FreeMonoBold12pt7b
        ),
        gunDescription(
            Element(ivec2(280,100), LuminaUI_AUTO, ivec2(200,200)),
            "Gun Description",     // content
            TFT_WHITE,             // textColor
            TFT_BLACK,             // fillColor
            TFT_WHITE,             // borderColor
            1,                     // textSize
            TC_DATUM,              // textDatum
            1.0f,                  // lineSpacing
            10,                    // cornerRadius
            &FreeMono9pt7b,        // freeFont
            true,                  // renderFill
            true                   // renderBorder
        ),        
         backButton(
           Element(ivec2(20,240), LuminaUI_AUTO, ivec2(200,70)),
           String("Back"), TFT_BLACK, TFT_CYAN, TFT_BLACK, 20,1, 0.0f,
           &FreeMono24pt7b, true, true
         ),
         nextButton(
            Element(ivec2(250,240), LuminaUI_AUTO, ivec2(200,70)),
            String("Next"), TFT_BLACK, TFT_ORANGE, TFT_BLACK, 20, 1,
            0.0f, &FreeMono24pt7b, true, true
         )

     {
         if (playerNumber == 1) {
             prevTriangle.OnTouch_setHandler(onPlayer1PrevGunButtonTouch);
             prevTriangle.OnTouch_setEnable(true);
             nextTriangle.OnTouch_setHandler(onPlayer1NextGunButtonTouch);
             nextTriangle.OnTouch_setEnable(true);
             backButton.OnTouch_setHandler(onPlayer1GunSetupBackButtonTouch);
             backButton.OnTouch_setEnable(true);
            nextButton.OnTouch_setHandler(onPlayer1GunSetupNextButtonTouch);
            nextButton.OnTouch_setEnable(true);
         } else if (playerNumber == 2) {
             prevTriangle.OnTouch_setHandler(onPlayer2PrevGunButtonTouch);
             prevTriangle.OnTouch_setEnable(true);
             nextTriangle.OnTouch_setHandler(onPlayer2NextGunButtonTouch);
             nextTriangle.OnTouch_setEnable(true);
                backButton.OnTouch_setHandler(onPlayer2GunSetupBackButtonTouch);
                backButton.OnTouch_setEnable(true);
            nextButton.OnTouch_setHandler(onPlayer2GunSetupNextButtonTouch);
            nextButton.OnTouch_setEnable(true);
         }
 
         // Add all children
         Element* elems[] = {
                &background, &titleText,
                &prevTriangle, &gunName, &nextTriangle,
                &descriptionTitle, &gunDescription,
                &backButton, &nextButton
         };
         elements.addFromArray(elems, sizeof(elems)/sizeof(Element*));
     }
 
    Viewport render(const Viewport &vp) override {
        if (playerNumber == 1) {
            if (Game::player1.hasGun() == false)
            {
                GUI::selectActivity(GUI_Manager_Activity::PLAYER2GUNSETUP);
                return Viewport(ivec2(0, 0), ivec2(0, 0));
            }
        } else if (playerNumber == 2) {
            if (Game::player2.hasGun() == false)
            {
                moveToRSG();
                return Viewport(ivec2(0, 0), ivec2(0, 0));
            }
        }
        gunName.content = String(gunDataNameArray[gunIndex]);
        return Activity::render(vp);
    }
 };
 
 // Instances of PlayerSetup for each player
static GunSetup *player1GunSetup = new GunSetup(1);
static GunSetup *player2GunSetup = new GunSetup(2);
 
 // Touch handlers - Player 1:
 /**
  * @brief Touch event handler for the Previous gun button - Player 1.
  */
 void onPlayer1PrevGunButtonTouch(ivec2, TouchStatus)
 {
     // Decrement gun index and wrap around if necessary
     player1GunSetup->gunIndex = (player1GunSetup->gunIndex - 1 + gunDataArraySize) % gunDataArraySize;
     player1GunSetup->callRender();
 }

    /**
    * @brief Touch event handler for the Next gun button - Player 1.
    */
void onPlayer1NextGunButtonTouch(ivec2, TouchStatus)
{
    // Increment gun index and wrap around if necessary
    player1GunSetup->gunIndex = (player1GunSetup->gunIndex + 1) % gunDataArraySize;
    player1GunSetup->callRender();
}

    /**
    * @brief Touch event handler for the Back button - Player 1.
    */
 void onPlayer1GunSetupBackButtonTouch(ivec2 point, TouchStatus status) {
    switch (status) {
        case TouchStatus::TouchStatus_RELEASE:
            player1GunSetup->backButton.background.fillColor = TFT_CYAN;
            player1GunSetup->backButton.background.borderColor = TFT_BLACK;
            player1GunSetup->backButton.text.textColor = TFT_BLACK;
            player1GunSetup->backButton.callRender();

            // Go back to the previous activity
            GUI::selectActivity(GUI_Manager_Activity::PLAYERSETUP);
            break;

        case TouchStatus::TouchStatus_PRESS:
            // Change the button color when pressed
            player1GunSetup->backButton.background.fillColor = TFT_BLUE;
            player1GunSetup->backButton.background.borderColor = TFT_WHITE;
            player1GunSetup->backButton.text.textColor = TFT_WHITE;
            player1GunSetup->backButton.callRender();
            break;

        case TouchStatus::TouchStatus_READY:
            // Change the button color back when released
            player1GunSetup->backButton.background.fillColor = TFT_CYAN;
            player1GunSetup->backButton.background.borderColor = TFT_BLACK;
            player1GunSetup->backButton.text.textColor = TFT_BLACK;
            player1GunSetup->backButton.callRender();
            break;

        default:
            break;
    }
}

/**
 * @brief Touch event handler for the Next button - Player 1.
 */
void onPlayer1GunSetupNextButtonTouch(ivec2 point, TouchStatus status) {
        switch (status) {
            case TouchStatus::TouchStatus_RELEASE:
                player1GunSetup->nextButton.background.fillColor = TFT_ORANGE;
                player1GunSetup->nextButton.background.borderColor = TFT_BLACK;
                player1GunSetup->nextButton.text.textColor = TFT_BLACK;
                player1GunSetup->nextButton.callRender();

                // Proceed to the next step
                Game::player1.setGunData(gunDataArray[player1GunSetup->gunIndex]);
                GUI::selectActivity(GUI_Manager_Activity::PLAYER2GUNSETUP);
                break;

            case TouchStatus::TouchStatus_PRESS:
                // Change the button color when pressed
                player1GunSetup->nextButton.background.fillColor = TFT_MAROON;
                player1GunSetup->nextButton.background.borderColor = TFT_YELLOW;
                player1GunSetup->nextButton.text.textColor = TFT_YELLOW;
                player1GunSetup->nextButton.callRender();
                break;

            case TouchStatus::TouchStatus_READY:
                // Change the button color back when released
                player1GunSetup->nextButton.background.fillColor = TFT_ORANGE;
                player1GunSetup->nextButton.background.borderColor = TFT_BLACK;
                player1GunSetup->nextButton.text.textColor = TFT_BLACK;
                player1GunSetup->nextButton.callRender();
                break;
             
            default:
                break;
        }
}

// Touch handlers - Player 2:
/**
 * @brief Touch event handler for the Previous gun button - Player 2.
 */
void onPlayer2PrevGunButtonTouch(ivec2, TouchStatus)
{
    // Decrement gun index and wrap around if necessary
    player2GunSetup->gunIndex = (player2GunSetup->gunIndex - 1 + gunDataArraySize) % gunDataArraySize;
    player2GunSetup->callRender();
}
/**
 * @brief Touch event handler for the Next gun button - Player 2.
 */
void onPlayer2NextGunButtonTouch(ivec2, TouchStatus)
{
    // Increment gun index and wrap around if necessary
    player2GunSetup->gunIndex = (player2GunSetup->gunIndex + 1) % gunDataArraySize;
    player2GunSetup->callRender();
}
/**
 * @brief Touch event handler for the Back button - Player 2.
 */
void onPlayer2GunSetupBackButtonTouch(ivec2 point, TouchStatus status) {
    switch (status) {
        case TouchStatus::TouchStatus_RELEASE:
            player2GunSetup->backButton.background.fillColor = TFT_CYAN;
            player2GunSetup->backButton.background.borderColor = TFT_BLACK;
            player2GunSetup->backButton.text.textColor = TFT_BLACK;
            player2GunSetup->backButton.callRender();

            // Go back to the previous activity
            GUI::selectActivity(GUI_Manager_Activity::PLAYER1GUNSETUP);
            break;

        case TouchStatus::TouchStatus_PRESS:
            // Change the button color when pressed
            player2GunSetup->backButton.background.fillColor = TFT_BLUE;
            player2GunSetup->backButton.background.borderColor = TFT_WHITE;
            player2GunSetup->backButton.text.textColor = TFT_WHITE;
            player2GunSetup->backButton.callRender();
            break;

        case TouchStatus::TouchStatus_READY:
            // Change the button color back when released
            player2GunSetup->backButton.background.fillColor = TFT_CYAN;
            player2GunSetup->backButton.background.borderColor = TFT_BLACK;
            player2GunSetup->backButton.text.textColor = TFT_BLACK;
            player2GunSetup->backButton.callRender();
            break;

        default:
            break;
    }
}

/**
 * @brief Touch event handler for the Next button - Player 2.
 */
void onPlayer2GunSetupNextButtonTouch(ivec2 point, TouchStatus status) {
    switch (status) {
        case TouchStatus::TouchStatus_RELEASE:
            player2GunSetup->nextButton.background.fillColor = TFT_ORANGE;
            player2GunSetup->nextButton.background.borderColor = TFT_BLACK;
            player2GunSetup->nextButton.text.textColor = TFT_BLACK;
            player2GunSetup->nextButton.callRender();

            // Proceed to the next step
            Game::player2.setGunData(gunDataArray[player2GunSetup->gunIndex]);
            moveToRSG();
            break;

        case TouchStatus::TouchStatus_PRESS:
            // Change the button color when pressed
            player2GunSetup->nextButton.background.fillColor = TFT_MAROON;
            player2GunSetup->nextButton.background.borderColor = TFT_YELLOW;
            player2GunSetup->nextButton.text.textColor = TFT_YELLOW;
            player2GunSetup->nextButton.callRender();
            break;

        case TouchStatus::TouchStatus_READY:
            // Change the button color back when released
            player2GunSetup->nextButton.background.fillColor = TFT_ORANGE;
            player2GunSetup->nextButton.background.borderColor = TFT_BLACK;
            player2GunSetup->nextButton.text.textColor = TFT_BLACK;
            player2GunSetup->nextButton.callRender();
            break;

        default:
            break;
    }
}
 
 #endif // GUNSETUP_HPP 