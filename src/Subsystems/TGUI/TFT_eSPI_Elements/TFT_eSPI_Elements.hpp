#include <TFT_eSPI.h>

TFT_eSprite * img_instance = nullptr;

void push_TFT_eSPI()
{
    img_instance->pushSprite(0,0);
}

// Drivers
#include "Touch_XPT2046.hpp"

// Elements
#include "Shape.hpp"