#ifndef TEXT_HPP
#define TEXT_HPP

#include "TFT_eSPI_Elements.hpp"
#include <TFT_eSPI.h>

/**
 * @brief A text element that displays text on the screen.
 * 
 * Datums:
 * - TL_DATUM: Top left
 * - TC_DATUM: Top center
 * - TR_DATUM: Top right
 * - ML_DATUM: Middle left
 * - MC_DATUM: Middle center
 * - MR_DATUM: Middle right
 * - BL_DATUM: Bottom left
 * - BC_DATUM: Bottom center
 * - BR_DATUM: Bottom right
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

    // New member to store the processed lines.
    HyperList<String> lines;

    Text(const Element& element, const String& content = "", uint32_t textColor = TFT_WHITE,
         uint8_t textSize = 1, uint8_t textDatum = TL_DATUM, float lineSpacing = 1.0f, const GFXfont *freeFont = FF1,
         uint32_t backgroundColor = TFT_BLACK, bool renderBackground = false)
        : Element(element), content(content), textColor(textColor), textSize(textSize), 
          textDatum(textDatum), lineSpacing(lineSpacing), freeFont(freeFont),
          backgroundColor(backgroundColor), renderBackground(renderBackground) {}

    Viewport render(const Viewport& viewport) override {
        Viewport textViewport = Element::render(viewport);
        
        if (content.isEmpty()) {
            return textViewport;
        }
        
        // Set the viewport to the element's position and size.
        LuminaUI::tft_instance->setViewport(textViewport.position.x, textViewport.position.y, 
                                              textViewport.scale.x, textViewport.scale.y);

        // Configure text rendering properties.
        LuminaUI::tft_instance->setTextDatum(textDatum);
        LuminaUI::tft_instance->setTextSize(textSize);
        LuminaUI::tft_instance->setFreeFont(freeFont);
        if (renderBackground) {
            LuminaUI::tft_instance->setTextColor(textColor, backgroundColor);
        } else {
            LuminaUI::tft_instance->setTextColor(textColor);
        }

        // Enable text wrapping in the TFT library (we're handling our own word wrapping below).
        LuminaUI::tft_instance->setTextWrap(true, true);

        // Calculate line height based on font height and line spacing.
        int lineHeight = LuminaUI::tft_instance->fontHeight() * lineSpacing;
        // Calculate starting X and Y based on the datum.
        int16_t cursorX = min(textDatum % 3, 2) * textViewport.scale.x / 2;
        int16_t cursorY = min(textDatum / 3, 2) * textViewport.scale.y / 2;
        
        // Only process multiline text for top datums (TL_DATUM, TC_DATUM, TR_DATUM).
        if (textDatum <= TR_DATUM) {
            lines.clear();
            int maxWidth = textViewport.scale.x;
            int startIndex = 0;
            int contentLen = content.length();
            
            // First, split content by newline characters.
            while (startIndex < contentLen) {
                int newlineIndex = content.indexOf('\n', startIndex);
                String rawLine;
                if (newlineIndex == -1) {
                    rawLine = content.substring(startIndex);
                    startIndex = contentLen;
                } else {
                    rawLine = content.substring(startIndex, newlineIndex);
                    startIndex = newlineIndex + 1;
                }
                
                // Word-wrap the raw line if its width exceeds the available viewport width.
                while (LuminaUI::tft_instance->textWidth(rawLine) > maxWidth) {
                    int breakPos = 0;
                    for (int j = 1; j <= rawLine.length(); j++) {
                        String subStr = rawLine.substring(0, j);
                        if (LuminaUI::tft_instance->textWidth(subStr) > maxWidth) {
                            breakPos = j - 1;
                            break;
                        }
                        if (j == rawLine.length()) {
                            breakPos = j;
                        }
                    }
                    // Attempt to break at the last space for better word separation.
                    int spacePos = rawLine.lastIndexOf(' ', breakPos);
                    if (spacePos != -1 && spacePos > 0) {
                        breakPos = spacePos;
                    }
                    // Add the fitting substring to our list of lines.
                    String subLine = rawLine.substring(0, breakPos);
                    lines.addend(subLine);
                    
                    // Remove the rendered substring and trim any leading spaces.
                    rawLine = rawLine.substring(breakPos);
                    while (rawLine.startsWith(" ")) {
                        rawLine = rawLine.substring(1);
                    }
                }
                // Add any remaining text from the current raw line.
                if (rawLine.length() > 0) {
                    lines.addend(rawLine);
                }
            }
            
            // Render each processed line.
            if (lines.size() > 1) {
                for (int i = 0; i < lines.size(); i++) {
                    LuminaUI::tft_instance->drawString(lines[i], cursorX, cursorY);
                    cursorY += lineHeight;
                }
            }
        } 
        
        if (lines.size() < 2) {
            // For non-top datums, render the content in a single line.
            LuminaUI::tft_instance->drawString(content, cursorX, cursorY);
        }
        
        // Reset the viewport after rendering.
        LuminaUI::tft_instance->resetViewport();
        return textViewport;
    }
};

#endif // TEXT_HPP