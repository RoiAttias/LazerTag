#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "TFT_eSPI_Elements.h"
#include <TFT_eSPI.h>

class Background : public Element
{
public:
    uint32_t fillColor;

    Background(uint32_t fillColor = TFT_BLACK)
    : Element(), fillColor(fillColor) {}

    Viewport render(const Viewport &viewport) override {
        // Call the base class's render function
        Element::render(viewport); // Updating _shouldRender flag, no need to clamp the viewport
        LuminaUI::tft_instance->setViewport(viewport.position.x, viewport.position.y, viewport.scale.x, viewport.scale.y);
        if (fillColor != TFT_TRANSPARENT) {
            LuminaUI::tft_instance->fillScreen(fillColor);
        }
        LuminaUI::tft_instance->resetViewport();
        return viewport;
    }
};


#endif // BACKGROUND_HPP