#ifndef TFT_ESPI_ELEMENTS_HPP
#define TFT_ESPI_ELEMENTS_HPP

#include <TFT_eSPI.h>

TFT_eSPI * tft_instance = nullptr;
TFT_eSprite * img_instance = nullptr;

void push_TFT_eSprite()
{
    img_instance->pushSprite(0,0);
}

// Drivers
#include "Touch_XPT2046.hpp"

// Elements
//#include "Shape.hpp"

#endif // TFT_ESPI_ELEMENTS_HPP