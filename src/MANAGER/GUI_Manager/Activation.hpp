/**
 * @file Activation.hpp
 * @brief “Activation” screen Activity for the Manager GUI.
 *
 * This Activity displays the project title, a prompt to start, and author credit.
 * When the user taps the screen, it schedules a transition to the Scanner screen
 * after a short delay and broadcasts the current game status to all devices.
 */

 #ifndef ACTIVATION_HPP
 #define ACTIVATION_HPP
 
 #include "GUI_Manager.hpp"
 #include "Utilities/Countdowner.hpp"
 #include "Utilities/MoreMath.hpp"
 #include "MANAGER/manager_shared.hpp"

 // From Scanner.hpp
 void triggerScanner(); ///< Forward declaration for scanner trigger function
 
 /**
  * @class Activation
  * @brief Initial welcome screen for the Manager device.
  *
  * This Activity shows a colored background, the project name ("Project LazerTag"),
  * a “PRESS ANYWHERE TO PLAY” prompt, and an author credit. Touching the screen
  * starts a countdown event that switches to the Scanner Activity and informs all
  * networked devices of the current GAME_WAITING status.
  */
 class Activation : public Activity {
 public:
     Background background;    ///< Full-screen background colored block
     Text text1;               ///< Displays "Project"
     Text text2;               ///< Displays "LazerTag"
     Text text3;               ///< Displays "PRESS ANYWHERE TO PLAY"
     Text text4;               ///< Displays "Made by Roi Attias"
     Text text5;               ///< Displays version in pormat "vX.Y.Z"
 
     uint32_t countdownTime = 400;  ///< Delay before transitioning to scanner (ms)
 
     /**
      * @brief Construct the Activation Activity.
      *
      * - Initializes the background to purple.
      * - Creates four Text elements at fixed Y positions with center alignment.
      * - Adds all elements to the base Activity's element list.
      */
     Activation()
       : Activity(),
         background(TFT_PURPLE),
         text1(
           Element(ivec2(0,  30), LuminaUI_AUTO, ivec2(480,  50)),
           String("Project"), TFT_WHITE, 1, TC_DATUM, 0.0f, &FreeMonoBold18pt7b
         ),
         text2(
           Element(ivec2(0,  80), LuminaUI_AUTO, ivec2(480,  70)),
           String("LazerTag"), TFT_WHITE, 2, TC_DATUM, 0.0f, &FreeMonoBold18pt7b
         ),
         text3(
           Element(ivec2(0, 180), LuminaUI_AUTO, ivec2(480,  50)),
           String("PRESS ANYWHERE TO PLAY"), TFT_WHITE, 1, TC_DATUM, 0.0f, &FreeMono12pt7b
         ),
         text4(
           Element(ivec2(0, 250), LuminaUI_AUTO, ivec2(480,  50)),
           String("Made by Roi Attias"), TFT_WHITE, 1, TC_DATUM, 0.0f, &FreeMonoBold12pt7b
         ),
         text5(
           Element(ivec2(0, 290), LuminaUI_AUTO, ivec2(480,  30)),
           SYSTEM_VERSION_STRING + "\0", TFT_WHITE, 1, MC_DATUM, 0.0f, &FreeMono9pt7b
         )
     {
         // Register all elements in the Activity's element list
         Element* elems[] = { &background, &text1, &text2, &text3, &text4 , &text5 };
         elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));
     }
 
     /**
      * @brief Render the Activation screen.
      *
      * Just calls the base Activity::render to draw all child elements.
      *
      * @param viewport Clipping viewport for rendering.
      * @return The portion of the screen actually rendered (same as input).
      */
     Viewport render(const Viewport &viewport) override {
         return Activity::render(viewport);
     }
 
     /**
      * @brief Handle touch events on the Activation screen.
      *
      * On a PRESS event, schedules a transition to the Scanner Activity after
      * @c countdownTime milliseconds and broadcasts the GAME_WAITING status
      * to all peers on the Nexus network.
      *
      * @param point The touch location (ignored).
      * @param touchStatus The type of touch event (only PRESS is handled).
      */
     void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override {
         if (touchStatus == TouchStatus_PRESS) {
             // After delay, switch to Scanner screen
             countdowner->addEvent(
               countdownTime,
               GUI::selectActivity,
               GUI_Manager_Activity::SCANNER
             );
             // Inform all devices that we are still in the waiting state
             Nexus::sendData(
               COMMS_GAMESTATUS,
               payloadSizePerCommand[COMMS_GAMESTATUS],
               (uint8_t *)&Game::status,
               NexusAddress(NEXUS_PROJECT_ID, 0xFF, 0xFF)
             );
              // Trigger the scanner to start scanning if not the first scan
              //    and set the flag for the next scans
             if (notTheFirstScan) {
              triggerScanner();
             } else {
                 notTheFirstScan = true;
             }
         }
     }
 };
 
 /// Global Activation Activity instance
 static Activation *activation = new Activation();
 
 #endif // ACTIVATION_HPP 