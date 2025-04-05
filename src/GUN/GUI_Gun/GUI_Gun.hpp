#ifndef GUI_GUN_HPP
#define GUI_GUN_HPP

#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define LUMINAUI_USE_U8G2
#include "Components/LuminaUI/LuminaUI.hpp"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

// Paths to Activities
#include "OnGame.hpp"
#include "MessageBox.hpp"

enum GUI_Gun_Activity : uint8_t {
    ONGAME,
    MESSAGEBOX,
    GUI_Gun_Activity_size
};
// Activities
Activity * GUI_Gun_Activities[] = {
    onGame,
    messageBox
};

ivec2 screenDiamentions(128, 64);
Screen screen2(false);

namespace GUI {
    inline void init(Player *playerPtr, Gun *gunPtr) {
        // OLED initialization
        u8g2.begin();

        // LuminaUI initialization
        screen2.init(screenDiamentions, GUI_Gun_Activities, GUI_Gun_Activity_size, true);

        // Player initialization
        onGame->setPointers(playerPtr, gunPtr);

        u8g2.clearBuffer();
    }
    
    inline void loop() {
        if (screen2.shouldRender()) {
            u8g2.clearBuffer();
            screen2.render();
            u8g2.sendBuffer();
        }
    }

    inline void callRender() {
        screen2.callRender();
    }

    inline void onGame() {
        screen2.selectActivity(GUI_Gun_Activity::ONGAME);
    }

    inline void message(String message) {
        messageBox->setMessage(message);
        screen2.selectActivity(GUI_Gun_Activity::MESSAGEBOX);
    }
}

#endif // GUI_GUN_HPP