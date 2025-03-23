#ifndef MESSAGE_BOX_HPP
#define MESSAGE_BOX_HPP

#include "GUI_Gun.hpp"
#include <Arduino.h>

class MessageBoxActivity : public Activity {
public:
    // The message to display. (Three words in this example)
    String message;

    // Constructor with default three-word message.
    MessageBoxActivity() : Activity(), message("MESSAGE BOX ACTIVE") {}

    // Render function that draws a rectangle barrier and big centered text.
    Viewport render(const Viewport &viewport) override {
        update();

        // Define margin for the rectangle barrier.
        int margin = 10;
        int rectX = margin;
        int rectY = margin;
        int rectWidth = u8g2.getDisplayWidth() - 2 * margin;
        int rectHeight = u8g2.getDisplayHeight() - 2 * margin;
        
        // Draw rectangle barrier (frame)
        u8g2.drawFrame(rectX, rectY, rectWidth, rectHeight);

        // Set a large font for the text.
        u8g2.setFont(u8g2_font_fub11_tf);
        u8g2.setFontPosCenter();
        
        // Calculate position to center the text.
        int textWidth = u8g2.getStrWidth(message.c_str());
        int x = (u8g2.getDisplayWidth() - textWidth) / 2;
        int y = u8g2.getDisplayHeight() / 2;
        
        // Draw the text.
        u8g2.setCursor(x, y);
        u8g2.print(message.c_str());

        // Optionally, you can call the parent render method.
        return Activity::render(viewport);
    }

    void setMessage(String message) {
        this->message = message;
    }

    // Update function (for future modifications or dynamic changes)
    void update() {
        // No dynamic update is needed now.
    }
};

// Instantiate the MessageBoxActivity.
MessageBoxActivity *messageBox = new MessageBoxActivity();

#endif // MESSAGE_BOX_HPP
