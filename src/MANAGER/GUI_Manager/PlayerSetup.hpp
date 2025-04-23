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
 
 // Forward declarations
 void onGunSwitchButtonTouch(ivec2, TouchStatus);
 void onVestSwitchButtonTouch(ivec2, TouchStatus);
 void onPlayerSetupBackButtonTouch(ivec2, TouchStatus);
 void onPlayerSetupNextButtonTouch(ivec2, TouchStatus);
 
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
         // Configure switch & nav buttons
         gunSwitchButton.OnTouch_setHandler(onGunSwitchButtonTouch);
         gunSwitchButton.OnTouch_setEnable(true);
         vestSwitchButton.OnTouch_setHandler(onVestSwitchButtonTouch);
         vestSwitchButton.OnTouch_setEnable(true);
 
         backButton.OnTouch_setHandler(onPlayerSetupBackButtonTouch);
         backButton.OnTouch_setEnable(true);
         nextButton.OnTouch_setHandler(onPlayerSetupNextButtonTouch);
         nextButton.OnTouch_setEnable(true);
 
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
         for (auto &addr : GUI::gameDevices) {
             if (addr.groups == NEXUS_GROUP_GUN) {
                 (g++==0
                     ? Game::player1.setGunAddress(addr)
                     : Game::player2.setGunAddress(addr)
                 );
             }
             if (addr.groups == NEXUS_GROUP_VEST) {
                 (v++==0
                     ? Game::player1.setVestAddress(addr)
                     : Game::player2.setVestAddress(addr)
                 );
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
 void onGunSwitchButtonTouch(ivec2, TouchStatus status) {
     if (status == TouchStatus_PRESS) playerSetup->switchGuns();
 }
 void onVestSwitchButtonTouch(ivec2, TouchStatus status) {
     if (status == TouchStatus_PRESS) playerSetup->switchVests();
 }
 void onPlayerSetupBackButtonTouch(ivec2, TouchStatus status) {
     if (status == TouchStatus_RELEASE) {
         playerSetup->backButton.background.fillColor = TFT_CYAN;
         GUI::selectActivity(GUI_Manager_Activity::SCANNER);
     }
 }
 void onPlayerSetupNextButtonTouch(ivec2, TouchStatus status) {
     if (playerSetup->canNext() && status == TouchStatus_RELEASE) {
         GUI::selectActivity(GUI_Manager_Activity::READYSETGO);
         Game::start();
         Nexus::sendData(COMMS_GAMESTATUS,
                         payloadSizePerCommand[COMMS_GAMESTATUS],
                         (uint8_t *)&Game::status,
                         NexusAddress{NEXUS_PROJECT_ID, 0xff, 0xff});
     }
 }
 
 #endif // PLAYERSETUP_HPP 