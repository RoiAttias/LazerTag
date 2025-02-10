#ifndef TEXT_HPP
#define TEXT_HPP

#include "TFT_eSPI_Elements.h"
#include <TFT_eSPI.h>

/**
 * @brief A text element that displays text on the screen.
 */
class Text : public Element {
public:
    String content;
    uint32_t textColor;
    uint8_t textSize;
    uint8_t textDatum;
    float lineSpacing;
    const GFXfont *freeFont;
    uint32_t backgroundColor;
    bool renderBackground;

    Text(const Element& element, const String& content = "", uint32_t textColor = TFT_WHITE,
         uint8_t textSize = 1, uint8_t textDatum = TL_DATUM, float lineSpacing = 1.0f, const GFXfont *freeFont = FF1,
         uint32_t backgroundColor = TFT_BLACK, bool renderBackground = false)
        : Element(element), content(content), textColor(textColor), textSize(textSize), 
          textDatum(textDatum), lineSpacing(lineSpacing), freeFont(freeFont), backgroundColor(backgroundColor), renderBackground(renderBackground) {}

    Viewport render(const Viewport& viewport) override {
    Viewport textViewport = Element::render(viewport);

    if (content.isEmpty()) {
        return textViewport;
    }

    // Set the viewport to the element's position and size
    LuminaUI::tft_instance->setViewport(textViewport.position.x, textViewport.position.y, 
                                   textViewport.scale.x, textViewport.scale.y);

    // Configure text rendering properties
    LuminaUI::tft_instance->setTextDatum(textDatum);
    LuminaUI::tft_instance->setTextSize(textSize);
    LuminaUI::tft_instance->setFreeFont(freeFont);
    if (renderBackground) {
            LuminaUI::tft_instance->setTextColor(textColor, backgroundColor);
    } else {
            LuminaUI::tft_instance->setTextColor(textColor);
    }

    // Enable text wrapping
    LuminaUI::tft_instance->setTextWrap(true, true);

    int lineHeight = LuminaUI::tft_instance->fontHeight() * lineSpacing;
    int16_t cursorX = min(textDatum % 3,2) * textViewport.scale.x / 2;
    int16_t cursorY = min(textDatum / 3,2) * textViewport.scale.y / 2;

    String currentText = ""; char c;
    for(int i = 0; i < content.length(); i++) {
        c = content[i];
        if (c != '\n') {
            currentText += c;
        }

        while (LuminaUI::tft_instance->textWidth(currentText) > textViewport.scale.x) {
            int endline = currentText.lastIndexOf(' ');
            if (endline == -1) {
                endline = currentText.length() - 1;
            }
            currentText = currentText.substring(0, endline);
            LuminaUI::tft_instance->drawString(currentText.substring(0, endline), cursorX, cursorY);
            cursorY += lineHeight;
            currentText = currentText.substring(endline);
        }

        if (c == '\n' || i == content.length() - 1) {
            LuminaUI::tft_instance->drawString(currentText, cursorX, cursorY);
            cursorY += lineHeight;
            currentText = "";
        }
    }

    if (currentText.length() > 0) {
        LuminaUI::tft_instance->drawString(currentText, cursorX, cursorY);
    }

    // Reset the viewport after rendering
    LuminaUI::tft_instance->resetViewport();

    // Return the viewport after rendering
    return textViewport;
}

};

#endif // TEXT_HPP