#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "TFT_eSPI_Elements.hpp"
#include <TFT_eSPI.h>

class Rectangle : public Element
{
public:
    uint32_t fillColor;
    uint32_t borderColor;
    bool drawBorder;
    int cornerRadius;

    Rectangle(Element element, uint32_t fillColor = TFT_TRANSPARENT, uint32_t borderColor = TFT_TRANSPARENT,
        bool drawBorder = false, int cornerRadius = 0): Element(element), fillColor(fillColor),
        borderColor(borderColor), drawBorder(drawBorder), cornerRadius(cornerRadius)  {}

    Viewport render(const Viewport &viewport) override {
        Viewport rectViewport = Element::render(viewport);
        TGUI::tft_instance->setViewport(rectViewport.position.x, rectViewport.position.y, rectViewport.scale.x, rectViewport.scale.y);
        if (fillColor != TFT_TRANSPARENT) {
            if (cornerRadius > 0) {
                TGUI::tft_instance->fillRoundRect(0, 0, rectViewport.scale.x, rectViewport.scale.y, cornerRadius, fillColor);
            } else {
                TGUI::tft_instance->fillRect(0, 0, rectViewport.scale.x, rectViewport.scale.y, fillColor);
            }
        }
        if (drawBorder && borderColor != TFT_TRANSPARENT) {
            if (cornerRadius > 0) {
                TGUI::tft_instance->drawRoundRect(0, 0, rectViewport.scale.x, rectViewport.scale.y, cornerRadius, borderColor);
            } else {
                TGUI::tft_instance->drawRect(0, 0, rectViewport.scale.x, rectViewport.scale.y, borderColor);
            }
        }
        TGUI::tft_instance->resetViewport();
        return rectViewport;
    }
};


#endif // ELEMENT_HPP