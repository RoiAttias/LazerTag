/**
 * @file Button.hpp
 * @brief Composite UI element representing an interactive button with styled background and centered text.
 *
 * Inherits from Textbox and defaults text alignment to center-middle (MC_DATUM), suitable for button labels.
 * Provides quick construction for clickable labels with background fill, border, and optional rounded corners.
 */

 #ifndef BUTTON_HPP
 #define BUTTON_HPP
 
 #include "TFT_eSPI_Elements.hpp"  ///< Global TFT and sprite instances
 #include "Textbox.hpp"           ///< Composite background + text element
 #include <TFT_eSPI.h>
 
 /**
  * @class Button
  * @brief Displays text inside a styled rectangle, centered by default.
  *
  * Extends Textbox, automatically setting the text datum to MC_DATUM (middle-center)
  * for button-style labels. Use to create clickable buttons with custom styling.
  */
 class Button : public Textbox {
 public:
     /**
      * @brief Construct a Button with specified styling and label.
      *
      * @param element      Base Element defining position and size.
      * @param content      Label text (default empty).
      * @param textColor    Text color (default TFT_WHITE).
      * @param fillColor    Background fill color (default TFT_BLACK).
      * @param borderColor  Background border color (default TFT_WHITE).
      * @param cornerRadius Corner radius for background (default 0).
      * @param textSize     Text size multiplier (default 1).
      * @param lineSpacing  Line spacing multiplier (default 0.0, single line).
      * @param freeFont     GFX FreeFont pointer (default FF1).
      * @param renderFill   Enable background fill (default true).
      * @param renderBorder Enable background border (default true).
      */
     Button(
         const Element& element,
         const String& content = "",
         uint32_t textColor = TFT_WHITE,
         uint32_t fillColor = TFT_BLACK,
         uint32_t borderColor = TFT_WHITE,
         int cornerRadius = 0,
         uint8_t textSize = 1,
         float lineSpacing = 0.0f,
         const GFXfont* freeFont = FF1,
         bool renderFill = true,
         bool renderBorder = true)
       : Textbox(
             element,
             content,
             textColor,
             fillColor,
             borderColor,
             textSize,
             MC_DATUM,      // Center-middle text alignment
             lineSpacing,
             cornerRadius,
             freeFont,
             renderFill,
             renderBorder)
     {}
 };
 
 #endif // BUTTON_HPP 