#ifndef ELLIPSE_HPP
#define ELLIPSE_HPP

#include "TFT_eSPI_Elements.hpp"
#include <TFT_eSPI.h>

class Ellipse : public Element
{
public:
    uint32_t fillColor;
    uint32_t borderColor;
    bool renderFill;
    bool renderBorder;

    Ellipse(const Element& element, uint32_t fillColor = TFT_WHITE, uint32_t borderColor = TFT_BLACK,
            bool renderFill = true, bool renderBorder = false) : Element(element), fillColor(fillColor),
        borderColor(borderColor), renderFill(renderFill), renderBorder(renderBorder) {}

    Viewport render(const Viewport &viewport) override {
        Viewport ellipseViewport = Element::render(viewport);
        TGUI::tft_instance->setViewport(ellipseViewport.position.x, ellipseViewport.position.y, ellipseViewport.scale.x, ellipseViewport.scale.y);

        int centerX = ellipseViewport.scale.x / 2;
        int centerY = ellipseViewport.scale.y / 2;
        int radiusX = ellipseViewport.scale.x / 2;
        int radiusY = ellipseViewport.scale.y / 2;

        if (renderFill) {
            if (radiusX != radiusY) {
                TGUI::tft_instance->fillEllipse(centerX, centerY, radiusX, radiusY, fillColor);
            } else {
                TGUI::tft_instance->fillCircle(centerX, centerY, radiusX, fillColor);
            }
        }

        if (renderBorder) {
            if (radiusX != radiusY) {
                TGUI::tft_instance->drawEllipse(centerX, centerY, radiusX, radiusY, borderColor);
            } else {
                TGUI::tft_instance->drawCircle(centerX, centerY, radiusX, borderColor);
            }
        }

        TGUI::tft_instance->resetViewport();
        return ellipseViewport;
    }
};

#endif // ELLIPSE_HPP
