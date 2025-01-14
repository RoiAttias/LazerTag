#ifndef TEXT_HPP
#define TEXT_HPP

#include "TFT_eSPI_Elements.hpp"
#include <TFT_eSPI.h>

class Text : public Element {
public:
    String content;
    uint32_t textColor;
    uint8_t textSize;
    uint8_t textDatum;
    float lineSpacing;
    int font;

    Text(const Element& element, const String& content = "", uint32_t textColor = TFT_WHITE, uint8_t textSize = 1, uint8_t textDatum = MC_DATUM, float lineSpacing = 1.0f, int font = 1)
        : Element(element), content(content), textColor(textColor), textSize(textSize), textDatum(textDatum), lineSpacing(lineSpacing), font(font) {}

    Viewport render(const Viewport& viewport) override {
        Viewport textViewport = Element::render(viewport);

        // Set the viewport
        TGUI::tft_instance->setViewport(textViewport.position.x, textViewport.position.y, textViewport.scale.x, textViewport.scale.y);

        // Set text properties
        TGUI::tft_instance->setTextDatum(textDatum);
        TGUI::tft_instance->setTextSize(textSize);
        TGUI::tft_instance->setTextColor(textColor);
        TGUI::tft_instance->loadFont(font); // Load the specified font

        int maxWidth = textViewport.scale.x;
        int yOffset = (TGUI::tft_instance.getTextDatum()/3) * TGUI::tft_instance->fontHeight() * textSize / 2;
        int xOffset = (TGUI::tft_instance.getTextDatum()%3) * maxWidth / 2;
        int lineHeight = TGUI::tft_instance->fontHeight() * textSize * lineSpacing;

        String currentLine;
        for (size_t i = 0; i < content.length(); ++i) {
            char c = content[i];
            // Check if the character is a space or newline or the last character
            if (c == ' ' || c == '\n' || i == content.length() - 1) {
                if (c != '\n' && i == content.length() - 1) {
                    currentLine += c; // Add the last character
                }
                // Check if the current line fits within the viewport
                if (TGUI::tft_instance->textWidth(currentLine) > maxWidth || c == '\n') {
                        TGUI::tft_instance->drawString(currentLine, 0, yOffset);
                        yOffset += lineHeight;
                        currentLine = "";
                    }
                }
                if (c != '\n') {
                    currentLine += c; // Start a new line if it's not a newline
                }
            } else {
                currentLine += c;
            }
        }

        // Draw any remaining text
        if (!currentLine.isEmpty()) {
            TGUI::tft_instance->drawString(currentLine, 0, yOffset);
        }

        // Reset the viewport
        TGUI::tft_instance->resetViewport();

        // Unload the font
        TGUI::tft_instance->unloadFont();

        return textViewport;
    }
};

#endif // TEXT_HPP