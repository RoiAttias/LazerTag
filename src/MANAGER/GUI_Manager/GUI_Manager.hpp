/**
 * @file GUI_Manager.hpp
 * @brief GUI subsystem for the Manager device using LuminaUI and TFT_eSPI.
 *
 * This header defines the graphical user interface components and workflow
 * for the Manager device. It initializes the TFT display, touch input,
 * and manages multiple Activities (screens) for device activation,
 * scanning, player setup, countdown, gameplay dashboard, etc.
 */

 #ifndef GUI_MANAGER_HPP
 #define GUI_MANAGER_HPP
 
 #include <Arduino.h>
 #include <TFT_eSPI.h>
 
 #define LUMINAUI_USE_TFT_ESPI  ///< Enable LuminaUI support for TFT_eSPI
 #include "Components/LuminaUI/LuminaUI.hpp"
 #include "Components/Nexus/Nexus.hpp"
 
 namespace GUI {
     //=== Hardware interfaces ===
     extern TFT_eSPI tft;               ///< TFT display driver instance
     extern ivec2 screenDiamentions;    ///< Screen resolution (width, height)
     extern Screen screen;              ///< LuminaUI Screen managing Activities
     extern Touch_XPT2046 touch;        ///< Touch driver for XPT2046 controller
 
     //=== Shared data ===
     extern HyperList<NexusAddress> gameDevices;  ///< List of discovered devices
 
     /**
      * @brief Initialize the GUI system.
      * @param startActivity The index of the Activity to display first.
      *
      * - Initializes the TFT display (rotation, clears screen).
      * - Initializes touch driver.
      * - Binds LuminaUI to the TFT instance.
      * - Initializes the Screen with all Activities.
      * - Enables press/release touch events.
      * - Selects the starting Activity.
      */
     void init(int startActivity = GUI_Manager_Activity::ACTIVATION);
 
     /**
      * @brief Switch to a specified GUI Activity.
      * @param activity Index of the Activity to select.
      */
     void selectActivity(int activity);
 
     /**
      * @brief Mark the current screen as needing redraw.
      */
     void callRender();
 
     /**
      * @brief Main loop for GUI:
      * - Polls touch input.
      * - Renders the current Activity when flagged.
      */
     void loop();
 }
 
 //=== GUI screens enumeration ===
 enum GUI_Manager_Activity : uint8_t {
     ACTIVATION,    ///< Device activation / wake-up screen
     SCANNER,       ///< Scans for remote devices
     PLAYERSETUP,   ///< Configures player parameters (addresses, weapon)
     READYSETGO,    ///< Countdown messages (Ready, Set, Go)
     GAMEPLAY,      ///< Live dashboard (HP bars, device status)
     GUI_Manager_Activity_size
 };
 
 //=== GUI Element includes ===
 #include "Elements/DeviceBox.hpp"   ///< Displays a discovered device
 #include "Elements/HpBar.hpp"       ///< Shows a player's health bar
 #include "Elements/Gradient.hpp"    ///< Background gradient element
 
 //=== GUI Activity includes ===
 #include "Activation.hpp"           ///< Activation screen Activity
 #include "Scanner.hpp"              ///< Device scanner Activity
 #include "PlayerSetup.hpp"          ///< Player setup Activity
 #include "Message.hpp"              ///< Simple message Activity
 #include "ReadySetGo.hpp"           ///< Countdown Activity
 #include "Gameplay.hpp"             ///< Gameplay dashboard Activity
 
 /// Array of all Activity pointers in presentation order
 extern Activity* GUI_Manager_Activities[/* GUI_Manager_Activity_size */];
 
 namespace GUI {
     // Definitions of extern variables
     TFT_eSPI tft = TFT_eSPI();
     ivec2 screenDiamentions;
     Screen screen(true);
     Touch_XPT2046 touch(&screen);
     HyperList<NexusAddress> gameDevices;
 
     Activity* GUI_Manager_Activities[GUI_Manager_Activity_size] = {
         activation,
         scanner,
         playerSetup,
         readySetGoMessage,
         gameplay
     };
 
     void init(int startActivity) {
         // TFT setup
         tft.begin();
         tft.setRotation(1);
         tft.fillScreen(TFT_BLACK);
         tft.getTouchRawZ();  // Initialize touch
 
         // Bind LuminaUI to TFT
         LuminaUI::tft_instance = &tft;
         screenDiamentions = ivec2(tft.width(), tft.height());
         screen.init(screenDiamentions,
                     GUI_Manager_Activities,
                     GUI_Manager_Activity_size,
                     true);
 
         // Setup touch input for press & release
         touch.init(ENABLE_PRESS | ENABLE_RELEASE);
 
         // Show the starting Activity
         selectActivity(startActivity);
     }
 
     void selectActivity(int activity) {
         screen.selectActivity(activity);
     }
 
     void callRender() {
         screen.callRender();
     }
 
     void loop() {
         touch.loop();
         if (screen.shouldRender()) {
             screen.render();
         }
     }
 };
 
 #endif // GUI_MANAGER_HPP 