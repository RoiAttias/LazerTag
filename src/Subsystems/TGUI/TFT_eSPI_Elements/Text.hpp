#ifndef TEXT_HPP
#define TEXT_HPP

#include "TFT_eSPI_Elements.hpp"
#include <TFT_eSPI.h>

/*
Font datum enumeration
These enumerate the text plotting alignment (reference datum point)
TL_DATUM 0 Top left (default)
TC_DATUM 1  Top centre
TR_DATUM 2  Top right
ML_DATUM 3  Middle left
CL_DATUM 3  Centre left, same as above
MC_DATUM 4  Middle centre
CC_DATUM 4  Centre centre, same as above
MR_DATUM 5  Middle right
CR_DATUM 5  Centre right, same as above
BL_DATUM 6  Bottom left
BC_DATUM 7  Bottom centre
BR_DATUM 8  Bottom right
L_BASELINE  9  Left character baseline (Line the 'A' character would sit on)
C_BASELINE 10 Centre character baseline
R_BASELINE 11  Right character baseline
*/

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

    Text(const Element& element, const String& content = "", uint32_t textColor = TFT_WHITE,
        uint8_t textSize = 1, uint8_t textDatum = TL_DATUM, float lineSpacing = 1.0f, const GFXfont *freeFont = FF0) 
        : Element(element), content(content), textColor(textColor), textSize(textSize), textDatum(textDatum),
        lineSpacing(lineSpacing), freeFont(freeFont) {}

    Viewport render(const Viewport& viewport) override {
        Viewport textViewport = Element::render(viewport);

        if (content.isEmpty()) {
            return textViewport;
        }
        
        // Set the viewport
        TGUI::tft_instance->setViewport(textViewport.position.x, textViewport.position.y, textViewport.scale.x, textViewport.scale.y);

        // Set text properties
        TGUI::tft_instance->setTextDatum(textDatum);
        TGUI::tft_instance->setTextSize(textSize);
        TGUI::tft_instance->setTextColor(textColor);
        TGUI::tft_instance->setFreeFont(freeFont);

        int maxWidth = textViewport.scale.x;
        int yOffset = min(textDatum/3,2) * TGUI::tft_instance->fontHeight() * textSize / 2;
        int xOffset = min(textDatum%3,2) * maxWidth / 2;
        int lineHeight = (int)(TGUI::tft_instance->fontHeight() * textSize * lineSpacing);

        String currentLine = "";
        for (size_t i = 0; i < content.length(); ++i) {
            char c = content[i];
            // Check if the character is a space or newline or the last character
            if (c == '\n' || i == content.length() - 1) {
                if (c != '\n' && i == content.length() - 1) {
                    currentLine += c;
                }
                // Check if the current line fits within the viewport
                if (TGUI::tft_instance->textWidth(currentLine) > maxWidth) {
                    int lastSpace = currentLine.lastIndexOf(' ');
                    if (lastSpace == -1) {
                        i = currentLine.length()-1;
                    } else {
                        i = lastSpace;
                    }
                    currentLine = currentLine.substring(0, lastSpace);
                }
                if (currentLine.isEmpty()) {
                    continue;
                }
                TGUI::tft_instance->drawString(currentLine, xOffset, yOffset);
                yOffset += lineHeight;
                currentLine = "";
            } else { 
                currentLine += c;
            }
        }

        // Draw any remaining text
        if (!currentLine.isEmpty()) {
            TGUI::tft_instance->drawString(currentLine, xOffset, yOffset);
        }

        // Reset the viewport
        TGUI::tft_instance->resetViewport();

        // Return the viewport
        return textViewport;
    }
};

#endif // TEXT_HPP