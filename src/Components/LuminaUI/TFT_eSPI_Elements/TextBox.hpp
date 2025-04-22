/**
 * @file TextBox.hpp
 * @brief Composite UI element combining a Rectangle background and Text content.
 *
 * Inherits from Element and composes a Rectangle for background and a Text element for
 * rendering text. Provides convenient styling with fill, border, corner radius, and text options.
 */

 #ifndef TEXTBOX_HPP
 #define TEXTBOX_HPP
 
 #include "TFT_eSPI_Elements.hpp"  ///< Global TFT and sprite instances
 #include "Rectangle.hpp"          ///< Background rectangle element
 #include "Text.hpp"               ///< Text rendering element
 #include <TFT_eSPI.h>
 
 /**
  * @class Textbox
  * @brief UI element displaying text inside a styled background.
  *
  * Members:
  *  - background: Rectangle element for background fill and border
  *  - text: Text element for drawing string content
  *
  * Constructor parameters allow full customization of background colors,
  * border, corner radius, text color, size, datum, line spacing, and font.
  */
 class Textbox : public Element {
 public:
     Rectangle background;  ///< Background rectangle styling
     Text text;             ///< Text content element
 
     /**
      * @brief Construct a Textbox element with combined background and text.
      *
      * @param element        Base Element defining origin, offset, and scale
      * @param content        Text string to display (default empty)
      * @param textColor      Color for text (default TFT_WHITE)
      * @param fillColor      Background fill color (default TFT_BLACK)
      * @param borderColor    Background border color (default TFT_WHITE)
      * @param textSize       Text size multiplier (default 1)
      * @param textDatum      Text alignment datum (default TL_DATUM)
      * @param lineSpacing    Line spacing multiplier (default 1.0)
      * @param cornerRadius   Corner radius for background (default 0)
      * @param freeFont       GFX FreeFont pointer (default FF1)
      * @param renderFill     Enable background fill (default true)
      * @param renderBorder   Enable background border (default true)
      */
     Textbox(
         const Element& element,
         const String& content = "",
         uint32_t textColor = TFT_WHITE,
         uint32_t fillColor = TFT_BLACK,
         uint32_t borderColor = TFT_WHITE,
         uint8_t textSize = 1,
         uint8_t textDatum = TL_DATUM,
         float lineSpacing = 1.0f,
         int cornerRadius = 0,
         const GFXfont* freeFont = FF1,
         bool renderFill = true,
         bool renderBorder = true)
       : Element(element)
       , background(element, fillColor, borderColor, renderFill, renderBorder, cornerRadius)
       , text(element, content, textColor, textSize, textDatum, lineSpacing, freeFont) {}
 
     /**
      * @brief Render the Textbox by drawing background then text.
      *
      * Calls base render to update flags, then renders background rectangle
      * and text content within the clipped viewport.
      *
      * @param viewport Clipping and offset rectangle for rendering.
      * @return The viewport used for this Textbox element.
      */
     Viewport render(const Viewport& viewport) override {
         Viewport tbVP = Element::render(viewport);
         // Draw background first
         tbVP = background.render(tbVP);
         // Draw text on top
         text.render(tbVP);
         return tbVP;
     }
 };
 
 #endif // TEXTBOX_HPP
 