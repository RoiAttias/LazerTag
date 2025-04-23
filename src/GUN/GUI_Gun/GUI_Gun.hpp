/**
 * @file GUI_Gun.hpp
 * @brief Top-level GUI subsystem for the Gun device using U8g2 and LuminaUI.
 *
 * This file sets up the OLED display, initializes LuminaUI Screen with two Activities
 * (OnGameActivity and MessageBoxActivity), and provides a simple namespace `GUI`
 * to drive which Activity is shown and trigger full-screen redraws.
 */

 #ifndef GUI_GUN_HPP
 #define GUI_GUN_HPP
 
 #include <U8g2lib.h>               ///< U8g2 OLED display library
 #ifdef U8X8_HAVE_HW_SPI
   #include <SPI.h>                 ///< SPI if hardware SPI is enabled
 #endif
 #ifdef U8X8_HAVE_HW_I2C
   #include <Wire.h>                ///< I2C if hardware I2C is enabled
 #endif
 
 #define LUMINAUI_USE_U8G2
 #include "Components/LuminaUI/LuminaUI.hpp"  ///< LuminaUI core: Activities, Elements, Screen
 
 /// Single global U8g2 instance for SSD1306 128×64 over I2C, no reset pin
 static U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
     U8G2_R0,           ///< display rotation
     /* reset=*/ U8X8_PIN_NONE
 );
 
 // Include the two Activity implementations
 #include "OnGame.hpp"            ///< OnGameActivity: shows HP, ammo, reload
 #include "MessageBox.hpp"        ///< MessageBoxActivity: shows framed message
 
 /// Identifiers for our GUI activities
 enum GUI_Gun_Activity : uint8_t {
     ONGAME,               ///< Gameplay HUD screen
     MESSAGEBOX,           ///< Transient message display
     GUI_Gun_Activity_size ///< Total number of activities
 };
 
 /// Array mapping enum to Activity pointers
 static Activity* GUI_Gun_Activities[GUI_Gun_Activity_size] = {
     onGame,               ///< OnGameActivity instance
     messageBox            ///< MessageBoxActivity instance
 };
 
 /// Screen dimensions (width, height) in pixels
 static ivec2 screenDimensions(128, 64);
 
 /// LuminaUI Screen instance managing our activities
 static Screen screen2(/* renderAfterOnTouch = */ false);
 
 namespace GUI {
 
 /**
  * @brief Initialize the GUI subsystem.
  * 
  * - Starts the U8g2 display.
  * - Initializes the LuminaUI Screen with activities.
  * - Assigns the Player and Gun pointers to OnGameActivity.
  * - Clears the display buffer for first render.
  *
  * @param playerPtr Pointer to the Player model.
  * @param gunPtr    Pointer to the Gun object.
  */
 inline void init(Player* playerPtr, Gun* gunPtr) {
     // Initialize OLED hardware
     u8g2.begin();
 
     // Initialize LuminaUI Screen
     screen2.init(
         screenDimensions,            ///< screen size
         GUI_Gun_Activities,          ///< array of activities
         GUI_Gun_Activity_size,       ///< number of activities
         /* enTouch=*/ true           ///< enable touch events (if used)
     );
 
     // Provide model pointers to the OnGame activity
     onGame->setPointers(playerPtr, gunPtr);
 
     // Clear buffer to prepare first frame
     u8g2.clearBuffer();
 }
 
 /**
  * @brief Main GUI loop function.
  * 
  * Checks if a redraw is requested via Screen.shouldRender(), and if so:
  *  - Clears the U8g2 buffer.
  *  - Invokes Screen.render() to draw current activity.
  *  - Sends buffer to the OLED.
  */
 inline void loop() {
     if (screen2.shouldRender()) {
         u8g2.clearBuffer();
         screen2.render();
         u8g2.sendBuffer();
     }
 }
 
 /**
  * @brief Request a full GUI redraw on next loop().
  */
 inline void callRender() {
     screen2.callRender();
 }
 
 /**
  * @brief Switch the Screen to the OnGame activity (gameplay HUD).
  */
 inline void onGame() {
     screen2.selectActivity(GUI_Gun_Activity::ONGAME);
 }
 
 /**
  * @brief Display a message using the MessageBox activity.
  * 
  * Sets the message text and switches activity.
  *
  * @param message The text string to display (up to three words recommended).
  */
 inline void message(const String& message) {
     messageBox->setMessage(message);
     screen2.selectActivity(GUI_Gun_Activity::MESSAGEBOX);
 }
 
 } // namespace GUI
 
 #endif // GUI_GUN_HPP 