#ifndef GUI_GUN_HPP
#define GUI_GUN_HPP

#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "Components/LuminaUI/LuminaUI.hpp"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

ivec2 screenDiamentions;
Screen screen(true);

// Paths to Activities


enum GUI_Gun_Activity : uint8_t {
    
    GUI_Gun_Activity_size
};

Activity * GUI_Gun_Activities[] = {
};

namespace GUI_Gun {
    void init() {
        LuminaUI::tft_instance = &u8g2;
        screenDiamentions = ivec2(LuminaUI::tft_instance->getDisplayWidth(), LuminaUI::tft_instance->getDisplayHeight());
        screen.init(screenDiamentions, GUI_Gun_Activities, GUI_Gun_Activity_size, true);
    }
}

#endif // GUI_GUN_HPP