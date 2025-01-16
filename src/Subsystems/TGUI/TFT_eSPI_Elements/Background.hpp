#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "TFT_eSPI_Elements.hpp"
#include <TFT_eSPI.h>

class Background : public Element
{
public:
    uint32_t fillColor;

    Rectangle(uint32_t fillColor = TFT_TRANSPARENT)
    : Element(), fillColor(fillColor) {}

    Viewport render(const Viewport &viewport) override {
        Viewport rectViewport = Element::render(viewport);
        TGUI::tft_instance->setViewport(rectViewport.position.x, rectViewport.position.y, rectViewport.scale.x, rectViewport.scale.y);
        int corRad = min(cornerRadius, min(rectViewport.scale.x, rectViewport.scale.y) / 2); // Radius cannot be larger than half the width or height
        if (fillColor != TFT_TRANSPARENT) {
            TGUI::tft_instance->fillScreen(fillColor);
        }
        TGUI::tft_instance->resetViewport();
        return rectViewport;
    }
};


#endif // BACKGROUND_HPP