#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "TFT_eSPI_Elements.hpp"
#include <TFT_eSPI.h>

class Rectangle : public Element
{
public:
    uint32_t fillColor;
    uint32_t borderColor;
    bool renderFill;
    bool renderBorder;
    int cornerRadius;

    Rectangle(Element element, uint32_t fillColor = TFT_WHITE, uint32_t borderColor = TFT_BLACK,
        bool renderFill = true, bool renderBorder = false, int cornerRadius = 0):
        Element(element), fillColor(fillColor), borderColor(borderColor),
        renderFill(renderFill), renderBorder(renderBorder), cornerRadius(cornerRadius) {}

    Viewport render(const Viewport &viewport) override {
        Viewport rectViewport = Element::render(viewport);
        LuminaUI::tft_instance->setViewport(rectViewport.position.x, rectViewport.position.y, rectViewport.scale.x, rectViewport.scale.y);
        int corRad = min(cornerRadius, min(rectViewport.scale.x, rectViewport.scale.y) / 2); // Radius cannot be larger than half the width or height
        if (renderFill) {
            if (corRad > 0) {
                LuminaUI::tft_instance->fillRoundRect(0, 0, rectViewport.scale.x, rectViewport.scale.y, corRad, fillColor);
            } else {
                LuminaUI::tft_instance->fillRect(0, 0, rectViewport.scale.x, rectViewport.scale.y, fillColor);
            }
        }
        if (renderBorder) {
            if (corRad > 0) {
                LuminaUI::tft_instance->drawRoundRect(0, 0, rectViewport.scale.x, rectViewport.scale.y, corRad, borderColor);
            } else {
                LuminaUI::tft_instance->drawRect(0, 0, rectViewport.scale.x, rectViewport.scale.y, borderColor);
            }
        }
        LuminaUI::tft_instance->resetViewport();
        return rectViewport;
    }
};


#endif // RECTANGLE_HPP