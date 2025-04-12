#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "GUI_Manager.hpp"

class Message : public Activity
{
public:
    // Elements
    Background background;     // A background element
    Text titleText;           // Title text displayed in the header
    Text messageText;         // Message text displayed in the body
    Button okButton;          // OK button to dismiss the message

    // Variables
    TouchEvent buttonHandler; // Touch event handler for the button

    /**
     * @brief Construct a Message activity.
     */
    Message(String title = "Title", String message = "This is a message.",
        String buttonText = "OK", TouchEvent buttonHandler = nullptr,
        uint32_t colorBG = TFT_BLACK, uint32_t colorB1 = TFT_YELLOW,
        uint32_t colorB2 = TFT_BLACK)
        : Activity(), buttonHandler(buttonHandler),
        background(colorBG),
        titleText(Element(ivec2(0, 10), LuminaUI_AUTO, ivec2(480, 50)),
                title, colorB1, 1, MC_DATUM, 0, &FreeMonoBold24pt7b),
        messageText(Element(ivec2(0, 70), LuminaUI_AUTO, ivec2(480, 200)),
                message, colorB1, 1, MC_DATUM, 0, &FreeMonoBold18pt7b),
        okButton(Element(ivec2(0, 260), LuminaUI_AUTO, ivec2(480, 50)),
                buttonText, colorB2, colorB1, colorB2, 10, 1, 0.0f)
    {
        if (buttonHandler != nullptr) {        
            okButton.OnTouch_setHandler(buttonHandler);
            okButton.OnTouch_setEnable(true);
            okButton.visible = true;
        } else {
            okButton.OnTouch_setEnable(false);
            okButton.visible = false;
        }

        // Add all elements to the activity.
        Element* elems[] = { &background, &titleText, &messageText, &okButton };
        elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));
    }
    /**
     * @brief Set the message text.
     * @param message The message to display.
     */
    void setMessage(const String& message) {
        messageText.content = message;
        messageText.callRender();
    }
    /**
     * @brief Set the title text.
     * @param title The title to display.
     */
    void setTitle(const String& title) {
        titleText.content = title;
        titleText.callRender();
    }
    void setButtonVisible(bool visible) {
        okButton.visible = visible;
        okButton.callRender();
    }
    /**
     * @brief Render the activity.
     * @param viewport The viewport to render the activity in.
     * @return The rendered viewport.
     */
    Viewport render(const Viewport &viewport) override {
        Viewport vp = Activity::render(viewport);
        return vp;
    }
    /**
     * @brief Update the activity.
     */
    void update() {
        // Update the activity if needed
    }
};

#endif // MESSAGE_HPP