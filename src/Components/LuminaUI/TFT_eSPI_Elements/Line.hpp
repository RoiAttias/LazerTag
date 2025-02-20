#ifndef LINE_HPP
#define LINE_HPP

#include "TFT_eSPI_Elements.hpp"

class Line : public Element {
public:
    enum Orientation { HORIZONTAL, VERTICAL };

    Orientation orientation;
    uint32_t color;

public:
    Line(const Element& element, Orientation orient, uint32_t lineColor, int length = 0)
        : Element(element), orientation(orient), color(lineColor) {
        if (length == 0) {
            if (orientation == HORIZONTAL) {
                length = scale.x;
            } else if (orientation == VERTICAL) {
                length = scale.y;
            }
        }
        if (orientation == HORIZONTAL) {
            scale = ivec2(length, 1);
        } else if (orientation == HORIZONTAL) {
            scale = ivec2(1, length);
        }
    }
    
    Viewport render(const Viewport &viewport) override {
        Viewport lineViewport = Element::render(viewport);
        LuminaUI::tft_instance->setViewport(lineViewport.position.x, lineViewport.position.y, lineViewport.scale.x, lineViewport.scale.y);
        ivec2 position = lineViewport.position;
        ivec2 lineLength = lineViewport.scale;
        if (orientation == HORIZONTAL) {
            LuminaUI::tft_instance->drawFastHLine(position.x, position.y, lineLength.x, color);
        } else if (orientation == VERTICAL) {
           LuminaUI::tft_instance->drawFastVLine(position.x, position.y, lineLength.y, color);
        }
        LuminaUI::tft_instance->resetViewport();
        return lineViewport;
    }
};

#endif // LINE_HPP