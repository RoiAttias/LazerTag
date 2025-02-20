#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "TFT_eSPI_Elements.hpp"
#include <TFT_eSPI.h>

class Textbox : public Element {
public:
    // Objects for the background (Rectangle) and text (Text)
    Rectangle background;
    Text text;
    int padding[4];

    Textbox(const Element& element, const String& content = "", uint32_t textColor = TFT_WHITE, uint32_t fillColor = TFT_BLACK,
            uint32_t borderColor = TFT_WHITE, uint8_t textSize = 1, uint8_t textDatum = TL_DATUM, float lineSpacing = 1.0f, int cornerRadius = 0,
            const GFXfont *freeFont = FF1, bool renderFill = true, bool renderBorder = true)
        : Element(element),
            background(element, fillColor, borderColor, renderFill, renderBorder, cornerRadius),
            text(element, content, textColor, textSize, textDatum, lineSpacing, freeFont) {}

    Viewport render(const Viewport& viewport) override {
        Viewport rectViewport = Element::render(viewport);
        // Render background (Rectangle)
        rectViewport = background.render(rectViewport);
        // Render text (Text)
        text.render(rectViewport);
        return rectViewport;
    }
};

#endif // TEXTBOX_HPP
