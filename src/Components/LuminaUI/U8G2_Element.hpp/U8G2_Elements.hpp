#ifndef U8G2_ELEMENTS_HPP
#define U8G2_ELEMENTS_HPP

#include "Components/LuminaUI/LuminaUI.hpp"
#include <TFT_eSPI.h>

namespace LuminaUI
{
    U8G2 * oled_instance;

    void push_oled()
    {
        oled_instance->sendBuffer();
    }
}

#endif // U8G2_ELEMENTS_HPP