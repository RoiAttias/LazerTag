/**
 * @file ReadySetGo.hpp
 * @brief “Ready, Set, Go!” countdown activity for the Manager GUI.
 *
 * Presents a modal dialog asking “Are you ready?” with a YES button.
 * On YES press, performs a 3–2–1–GO countdown, updates the game state,
 * broadcasts status to all devices, and then starts the game.
 */

 #ifndef READYSETGO_HPP
 #define READYSETGO_HPP
 
 #include "GUI_Manager.hpp"
 #include "Utilities/Countdowner.hpp"
 #include "MANAGER/manager_shared.hpp"
 
 /**
  * @brief Handler invoked at each countdown step (0=“3”,1=“2”,2=“1”,3=“GO!”,4=start).
  * @param parameter Current countdown index.
  *
  * - Parameters 0–3 update the displayed message and Game::status.
  * - Parameter 4 calls startGame() and transitions to GAMEPLAY.
  * - After updating status, broadcasts new status to all peers.
  * - Schedules itself for the next second using the Countdowner.
  */
 void countdownHandler(int parameter);
 
 /**
  * @brief Touch event handler for the YES button in the “Are you ready?” dialog.
  * @param point     Touch coordinates (unused).
  * @param status    TouchStatus indicating PRESS, RELEASE, etc.
  *
  * - On RELEASE: disables the button, starts the countdown at step 0.
  * - On PRESS: visually darkens the button to give feedback.
  * - On READY (i.e. finger released but before next press): restores button colors.
  */
 void readySetGoHandler(ivec2 point, TouchStatus status);
 
 /**
  * @brief Preconfigured Message activity prompting “Are you ready?” and YES button.
  *
  * - Background color: TFT_NAVY
  * - Title text color: TFT_YELLOW
  * - Button text on TFT_BLACK, fill TFT_YELLOW
  * - Button invokes readySetGoHandler on press/release.
  */
 Message *readySetGoMessage = new Message(
     "Are you ready?",     // Title
     "",                   // Body initially empty (set during countdown)
     "YES",                // Button label
     readySetGoHandler,    // Button callback
     TFT_NAVY,             // Background color
     TFT_YELLOW,           // Text color
     TFT_BLACK             // Button background/text secondary color
 );
 
 /** @brief The sequence of strings shown at each countdown step. */
 String readySetGoText[] = {
     "3",
     "2",
     "1",
     "GO!"
 };
 
 void countdownHandler(int parameter) {
     // If still within 0–3, update the dialog text and hide the button
     if (parameter < 4) {
         readySetGoMessage->setMessage(readySetGoText[parameter]);
         readySetGoMessage->setButtonVisible(false);
         GUI::callRender();
         // Schedule next countdown tick in 1 second
         countdowner->addEvent(1000, countdownHandler, parameter + 1);
     }
 
     // Advance the Game::status based on the countdown index
     switch (parameter) {
         case 0: Game::status = GameStatus::GAME_THREE; break;
         case 1: Game::status = GameStatus::GAME_TWO;   break;
         case 2: Game::status = GameStatus::GAME_ONE;   break;
         case 3: Game::status = GameStatus::GAME_GO;    break;
         case 4:
             startGame();      // Kick off the game
             delay(100);       // Brief pause before gameplay
             break;
     }
 
     // Broadcast the updated game status to all peers (broadcast MAC)
     Nexus::sendData(
         COMMS_GAMESTATUS,
         payloadSizePerCommand[COMMS_GAMESTATUS],
         (uint8_t *)&Game::status,
         NexusAddress(NEXUS_PROJECT_ID, 0xff, 0xff)
     );
 }
 
 void readySetGoHandler(ivec2 /*point*/, TouchStatus status) {
     if (status == TouchStatus::TouchStatus_RELEASE) {
         // On button release, start countdown at step 0
         countdowner->addEvent(100, countdownHandler, 0);
         // Disable further presses
         readySetGoMessage->okButton.OnTouch_setEnable(false);
         status = TouchStatus::TouchStatus_READY;
     }
     else if (status == TouchStatus::TouchStatus_PRESS) {
         // Darken the button while pressing
         readySetGoMessage->okButton.background.fillColor  = TFT_MAROON;
         readySetGoMessage->okButton.background.borderColor = TFT_YELLOW;
         readySetGoMessage->okButton.text.textColor        = TFT_YELLOW;
         GUI::callRender();
     }
 
     if (status == TouchStatus::TouchStatus_READY) {
         // Restore original button colors when ready again
         readySetGoMessage->okButton.background.fillColor  = TFT_YELLOW;
         readySetGoMessage->okButton.background.borderColor = TFT_BLACK;
         readySetGoMessage->okButton.text.textColor        = TFT_BLACK;
         GUI::callRender();
     }
 }
 
 #endif // READYSETGO_HPP 