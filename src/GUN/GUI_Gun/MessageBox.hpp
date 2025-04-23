/**
 * @file MessageBox.hpp
 * @brief Activity that displays a framed message centered on the OLED.
 *
 * Inherits from LuminaUI::Activity. Renders:
 *  - A rectangular border inset by a fixed margin.
 *  - A single-line message text using a large font, centered horizontally and vertically.
 */

 #ifndef MESSAGE_BOX_HPP
 #define MESSAGE_BOX_HPP
 
 #include "GUI_Gun.hpp"
 #include <Arduino.h>
 
 /**
  * @class MessageBoxActivity
  * @brief Activity that displays a framed message centered on the OLED.
  *
  * Inherits from LuminaUI::Activity. Renders:
  *  - A rectangular border inset by a fixed margin.
  *  - A single-line message text using a large font, centered horizontally and vertically.
  */
 class MessageBoxActivity : public Activity {
 public:
     /// The text to display in the message box.
     String message;
 
     /**
      * @brief Default constructor.
      * 
      * Initializes the base Activity and sets a default placeholder message.
      */
     MessageBoxActivity()
       : Activity(),
         message("MESSAGE BOX ACTIVE")
     {}
 
     /**
      * @brief Render the message box.
      * 
      * Draws a rectangular frame inset from the display edges, then centers
      * and prints the `message` string inside it.
      *
      * @param viewport  The LuminaUI viewport defining position & size.
      * @return          The clamped viewport used for this Activity.
      */
     Viewport render(const Viewport &viewport) override {
         // Let the base Activity clear its render flag
         update();
 
         // Define the margin between the frame and the display border
         const int margin = 10;
         int width  = u8g2.getDisplayWidth();
         int height = u8g2.getDisplayHeight();
         // Frame rectangle coordinates
         int rectX = margin;
         int rectY = margin;
         int rectW = width  - 2 * margin;
         int rectH = height - 2 * margin;
 
         // Draw the outer frame
         u8g2.drawFrame(rectX, rectY, rectW, rectH);
 
         // Configure a large font for the message text
         u8g2.setFont(u8g2_font_fub11_tf);
         u8g2.setFontPosCenter();
 
         // Calculate text width and centered coordinates
         int textW = u8g2.getStrWidth(message.c_str());
         int centerX = (width  - textW) / 2;
         int centerY = height / 2;
 
         // Draw the message
         u8g2.setCursor(centerX, centerY);
         u8g2.print(message.c_str());
 
         // Clear the render flag and clamp to the viewport
         return Activity::render(viewport);
     }
 
     /**
      * @brief Update internal state.
      *
      * Currently a placeholder, no dynamic data to refresh.
      */
     void update() {
         // Intentionally empty for future enhancements
     }
 
     /**
      * @brief Change the displayed message.
      * @param msg  New text to show in the message box.
      */
     void setMessage(const String &msg) {
         message = msg;
     }
 };
 
 // Create a single global instance on the heap
 static MessageBoxActivity *messageBox = new MessageBoxActivity();
 
 #endif // MESSAGE_BOX_HPP 