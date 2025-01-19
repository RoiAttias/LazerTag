#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "TFT_eSPI_Elements.h"
#include <TFT_eSPI.h>

class Button : public Textbox {
public:
    Button(const Element& element, const String& content = "", uint32_t textColor = TFT_WHITE, uint32_t fillColor = TFT_BLACK,
           uint32_t borderColor = TFT_WHITE, uint8_t textSize = 1, float lineSpacing = 0.0f, int cornerRadius = 0,
           const GFXfont *freeFont = FF0, bool renderFill = true, bool renderBorder = true)
        : Textbox(element, content, textColor, fillColor, borderColor, textSize, MC_DATUM,
            lineSpacing, cornerRadius, freeFont, nullptr, renderFill, renderBorder) {}
};

#endif // BUTTON_HPP