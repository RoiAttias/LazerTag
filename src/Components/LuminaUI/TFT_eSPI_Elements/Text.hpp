/**
 * @file Text.hpp
 * @brief UI element for rendering multi-line and single-line text with wrapping and datum alignment.
 *
 * Inherits from Element and uses the TFT_eSPI library to draw text within a clipped viewport.
 * Supports custom fonts (GFX FreeFonts), text size, color, background, datum alignment, and
 * configurable line spacing. Handles manual word wrapping and newline splitting for top-aligned text.
 */

 #ifndef TEXT_HPP
 #define TEXT_HPP
 
 #include "TFT_eSPI_Elements.hpp"  ///< Global TFT and sprite instances
 #include <TFT_eSPI.h>
 
 /**
  * @class Text
  * @brief Draws text strings with alignment, wrapping, and background support.
  *
  * Members:
  *  - content: The string to render (can include '\n' for explicit line breaks).
  *  - textColor: Foreground color for text.
  *  - textSize: Scale factor for bitmap fonts.
  *  - textDatum: Datum for alignment (TL, TC, TR, ML, MC, MR, BL, BC, BR).
  *  - lineSpacing: Multiplier for line height between wrapped lines.
  *  - freeFont: Pointer to a GFXfont for high-quality vector text.
  *  - backgroundColor: Background fill color when renderBackground is true.
  *  - renderBackground: Enable or disable background fill behind text.
  *  - lines: Internal list of wrapped lines for rendering.
  */
 class Text : public Element {
 public:
     String content;            ///< Text content to render
     uint32_t textColor;        ///< Text color (RGB565)
     uint8_t textSize;          ///< Text size multiplier for bitmap fonts
     uint8_t textDatum;         ///< Alignment datum constant
     float lineSpacing;         ///< Line spacing multiplier
     const GFXfont* freeFont;   ///< Pointer to GFX FreeFont (nullptr for bitmap)
     uint32_t backgroundColor;  ///< Background color behind text
     bool renderBackground;     ///< Whether to render background fill
 
     HyperList<String> lines;   ///< Wrapped lines computed at render time
 
     /**
      * @brief Constructs a Text element with full customization.
      *
      * @param element           Base Element (origin, scale, etc.).
      * @param content           Text to display (default empty).
      * @param textColor         Color of text (default TFT_WHITE).
      * @param textSize          Bitmap text size (default 1).
      * @param textDatum         Alignment datum (default TL_DATUM).
      * @param lineSpacing       Multiplier for line height (default 1.0).
      * @param freeFont          GFX FreeFont pointer (default FF1).
      * @param backgroundColor   Background fill color (default TFT_BLACK).
      * @param renderBackground  Enable background color (default false).
      */
     Text(
         const Element& element,
         const String& content = "",
         uint32_t textColor = TFT_WHITE,
         uint8_t textSize = 1,
         uint8_t textDatum = TL_DATUM,
         float lineSpacing = 1.0f,
         const GFXfont* freeFont = FF1,
         uint32_t backgroundColor = TFT_BLACK,
         bool renderBackground = false)
       : Element(element)
       , content(content)
       , textColor(textColor)
       , textSize(textSize)
       , textDatum(textDatum)
       , lineSpacing(lineSpacing)
       , freeFont(freeFont)
       , backgroundColor(backgroundColor)
       , renderBackground(renderBackground) {}
 
     /**
      * @brief Render text inside the given viewport.
      *
      * - Sets clipping to the element's viewport.
      * - Configures font, color, datum, size, and wrapping.
      * - Splits content by '\n' and performs manual word wrapping if necessary.
      * - Draws each line at computed cursor positions.
      * - Restores full viewport after drawing.
      *
      * @param viewport Clipping and offset rectangle for rendering.
      * @return The viewport used for this Text element.
      */
     Viewport render(const Viewport& viewport) override {
         Viewport textVP = Element::render(viewport);
         if (content.isEmpty()) return textVP;
 
         // Clip to Text element area
         LuminaUI::tft_instance->setViewport(
             textVP.position.x,
             textVP.position.y,
             textVP.scale.x,
             textVP.scale.y);
 
         // Configure font and rendering settings
         LuminaUI::tft_instance->setTextDatum(textDatum);
         LuminaUI::tft_instance->setTextSize(textSize);
         LuminaUI::tft_instance->setFreeFont(freeFont);
         if (renderBackground) {
             LuminaUI::tft_instance->setTextColor(textColor, backgroundColor);
         } else {
             LuminaUI::tft_instance->setTextColor(textColor);
         }
         LuminaUI::tft_instance->setTextWrap(true, true);
 
         // Compute line height
         int lineHeight = LuminaUI::tft_instance->fontHeight() * lineSpacing;
         // Initial cursor based on datum alignment
         int16_t cursorX = (textDatum % 3) * textVP.scale.x / 2;
         int16_t cursorY = (textDatum / 3) * textVP.scale.y / 2;
 
         // Manual wrapping and newline splitting for top-aligned datums
         if (textDatum <= TR_DATUM) {
             lines.clear();
             int maxW = textVP.scale.x;
             int idx = 0;
             while (idx < content.length()) {
                 int nl = content.indexOf('\n', idx);
                 String raw = (nl < 0)
                     ? content.substring(idx)
                     : content.substring(idx, nl);
                 idx = (nl < 0) ? content.length() : nl + 1;
 
                 // Word-wrap raw line
                 while (LuminaUI::tft_instance->textWidth(raw) > maxW) {
                     int br = 0;
                     for (int j = 1; j <= raw.length(); ++j) {
                         if (LuminaUI::tft_instance->textWidth(raw.substring(0,j)) > maxW) {
                             br = j - 1;
                             break;
                         }
                         if (j == raw.length()) br = j;
                     }
                     int sp = raw.lastIndexOf(' ', br);
                     if (sp > 0) br = sp;
                     lines.addend(raw.substring(0, br));
                     raw = raw.substring(br);
                     while (raw.startsWith(" ")) raw = raw.substring(1);
                 }
                 if (!raw.isEmpty()) lines.addend(raw);
             }
 
             // Draw each wrapped line
             for (int i = 0; i < lines.size(); ++i) {
                 LuminaUI::tft_instance->drawString(lines[i], cursorX, cursorY);
                 cursorY += lineHeight;
             }
         }
         
         // Single-line rendering for other datums or no wrapping
         if (lines.size() < 2) {
             LuminaUI::tft_instance->drawString(content, cursorX, cursorY);
         }
 
         // Restore viewport for subsequent draws
         LuminaUI::tft_instance->resetViewport();
         return textVP;
     }
 };
 
 #endif // TEXT_HPP 