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

enum GUI_Gun_Activity : uint8_t {
    ONGAME,
    GUI_Gun_Activity_size
};
// Activities
Activity * GUI_Gun_Activities[] = {
    onGame
};

ivec2 screenDiamentions(128, 64);
Screen screen(false);

namespace GUI {
    void init(Player *playerPtr) {
        // OLED initialization
        u8g2.begin();

        // LuminaUI initialization
        screen.init(screenDiamentions, GUI_Gun_Activities, GUI_Gun_Activity_size, true);
        screen.selectActivity(GUI_Gun_Activity::ONGAME);

        // Player initialization
        onGame->setPlayer(playerPtr);

        u8g2.clearBuffer();
    }
    
    void loop() {
        if (screen.shouldRender()) {
            u8g2.clearBuffer();
            screen.render();
            u8g2.sendBuffer();
        }
    }

    void callRender() {
        screen.callRender();
    }
}

#endif // GUI_GUN_HPP