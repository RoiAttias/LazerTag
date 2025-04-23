/**
 * @file Message.hpp
 * @brief Manager GUI “Message” activity for showing modal dialogs.
 *
 * Displays a full‐screen background, a title, a multi‐line message body, and an optional OK button.
 * Useful for alerts, confirmations, or status updates within the Manager interface.
 */

 #ifndef MESSAGE_HPP
 #define MESSAGE_HPP
 
 #include "GUI_Manager.hpp"
 
 /**
  * @class Message
  * @brief A generic modal dialog Activity with title, body text, and an OK button.
  *
  * Elements:
  *  - Background: solid fill of configurable color.
  *  - titleText: large, centered header text.
  *  - messageText: centered body text (multi‐line support via Text).
  *  - okButton: dismiss button with customizable label and handler.
  *
  * Usage:
  *   - Construct with desired title, message, button text, handler, and colors.
  *   - Add to GUI_Manager_Activities and switch to it via GUI::selectActivity().
  *   - Call setTitle()/setMessage() to update content dynamically.
  */
 class Message : public Activity
 {
 public:
     Background background;   ///< Solid background fill element.
     Text       titleText;    ///< Header text element.
     Text       messageText;  ///< Body text element.
     Button     okButton;     ///< OK button element.
     TouchEvent buttonHandler;///< Callback invoked when OK is pressed.
 
     /**
      * @brief Construct a new Message Activity.
      *
      * @param title          Text to display in the header.
      * @param message        Text to display in the body.
      * @param buttonText     Label for the OK button.
      * @param buttonHandler  Function to call on OK press (nullptr to hide button).
      * @param colorBG        Background color (e.g., TFT_BLACK).
      * @param colorB1        Primary color for title/message text.
      * @param colorB2        Secondary color for button background/text.
      */
     Message(String title       = "Title",
             String message     = "This is a message.",
             String buttonText  = "OK",
             TouchEvent buttonHandler = nullptr,
             uint32_t colorBG   = TFT_BLACK,
             uint32_t colorB1   = TFT_YELLOW,
             uint32_t colorB2   = TFT_BLACK)
       : Activity(),                  // Base class ctor
         buttonHandler(buttonHandler),
         background(colorBG),
         titleText(
           Element(ivec2(0, 10), LuminaUI_AUTO, ivec2(480,  50)),
           title, colorB1, 1, MC_DATUM, 0.0f, &FreeMonoBold24pt7b
         ),
         messageText(
           Element(ivec2(0, 70), LuminaUI_AUTO, ivec2(480, 200)),
           message, colorB1, 1, MC_DATUM, 0.0f, &FreeMonoBold18pt7b
         ),
         okButton(
           Element(ivec2(10,260), LuminaUI_AUTO, ivec2(460, 50)),
           buttonText, colorB2, colorB1, colorB2, 10, 1, 0.0f
         )
     {
         if (buttonHandler) {
             okButton.OnTouch_setHandler(buttonHandler);
             okButton.OnTouch_setEnable(true);
             okButton.visible = true;
         } else {
             okButton.OnTouch_setEnable(false);
             okButton.visible = false;
         }
 
         // Register elements in back-to-front order
         Element* elems[] = { &background, &titleText, &messageText, &okButton };
         elements.addFromArray(elems, sizeof(elems)/sizeof(Element*));
     }
 
     /**
      * @brief Update the body message and request a re-render.
      * @param message New message content.
      */
     void setMessage(const String& message) {
         messageText.content = message;
         messageText.callRender();
     }
 
     /**
      * @brief Update the header title and request a re-render.
      * @param title New title content.
      */
     void setTitle(const String& title) {
         titleText.content = title;
         titleText.callRender();
     }
 
     /**
      * @brief Show or hide the OK button and request a re-render.
      * @param visible True to show the button, false to hide.
      */
     void setButtonVisible(bool visible) {
         okButton.visible = visible;
         okButton.callRender();
     }
 
     /**
      * @brief Render callback, invoked each frame when active.
      * @param viewport Parent viewport defining the render area.
      * @return The clamped viewport after rendering.
      */
     Viewport render(const Viewport &viewport) override {
         // Base Activity::render will draw all registered child elements
         return Activity::render(viewport);
     }
 
     /**
      * @brief Placeholder for future per-frame updates (currently unused).
      */
     void update() {
         // No dynamic updates needed at present
     }
 };
 
 #endif // MESSAGE_HPP 