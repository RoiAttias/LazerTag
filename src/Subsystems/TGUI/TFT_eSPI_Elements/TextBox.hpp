#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "TFT_eSPI_Elements.hpp"s
#include "Rectangle.hpp"
#include "Text.hpp"
#include <TFT_eSPI.h>

class Textbox : public Element {
public:
    // Objects for the background (Rectangle) and text (Text)
    Rectangle background;
    Text text;
    int padding[4];

    Textbox(const Element& element, const String& content = "", uint32_t textColor = TFT_WHITE, uint32_t fillColor = TFT_BLACK,
            uint32_t borderColor = TFT_WHITE, uint8_t textSize = 1, uint8_t textDatum = TL_DATUM, float lineSpacing = 1.0f, int cornerRadius = 0,
            const GFXfont *freeFont = FF0, int padding[4] = nullptr, bool renderFill = true, bool renderBorder = true)
        : Element(element),
            background(element, fillColor, borderColor, renderFill, renderBorder, cornerRadius),
            text(element, content, textColor, textSize, textDatum, lineSpacing, freeFont) {
                if (padding != nullptr) {
                for (int i = 0; i < 4; i++) {
                    this->padding[i] = padding[i];
                }
                } else {
                for (int i = 0; i < 4; i++) {
                    this->padding[i] = 0;
                }
                }
                calculateTextMargin();
            }

    Viewport render(const Viewport& viewport) override {
        Viewport rectViewport = Element::render(viewport);
        // Render background (Rectangle)
        rect = background.render(rectViewport);
        // Render text (Text)
        calculateTextMargin();
        text.render(rectViewport);
        return rectViewport;
    }

    void calculateTextMargin() {
        text.marginAffectViewport = true;
        text.margin[0] = padding[0];
        text.margin[1] = padding[1];
        text.margin[2] = padding[2];
        text.margin[3] = padding[3];
    }
};

#endif // TEXTBOX_HPP
