/**
 * @file Background.hpp
 * @brief UI element for rendering a solid background color over the entire viewport.
 *
 * Inherits from Element and fills the screen (or assigned viewport) with a single color.
 */

 #ifndef BACKGROUND_HPP
 #define BACKGROUND_HPP
 
 #include "TFT_eSPI_Elements.hpp"  ///< Global TFT and sprite instances
 #include <TFT_eSPI.h>
 
 /**
  * @class Background
  * @brief Fills the element's viewport with a solid color.
  *
  * The Background element clears its clipping region by drawing a
  * full-screen (or full-viewport) rectangle of the specified fillColor.
  * Use TFT_TRANSPARENT to skip background fill.
  */
 class Background : public Element {
 public:
     uint32_t fillColor;  ///< Color to fill the background (RGB565)
 
     /**
      * @brief Constructs a Background element covering its viewport.
      * @param fillColor Color to use (default TFT_BLACK).
      */
     Background(uint32_t fillColor = TFT_BLACK)
       : Element(), fillColor(fillColor) {}
 
     /**
      * @brief Render the background by filling the viewport.
      *
      * Calls base render() to update the render flag, then sets clipping
      * to the provided viewport and fills it with fillColor (unless transparent).
      * Finally, restores the full display viewport.
      *
      * @param viewport Clipping and offset rectangle for rendering.
      * @return The same viewport passed in.
      */
     Viewport render(const Viewport &viewport) override {
         Element::render(viewport);
         LuminaUI::tft_instance->setViewport(
             viewport.position.x,
             viewport.position.y,
             viewport.scale.x,
             viewport.scale.y);
         if (fillColor != TFT_TRANSPARENT) {
             LuminaUI::tft_instance->fillScreen(fillColor);
         }
         LuminaUI::tft_instance->resetViewport();
         return viewport;
     }
 };
 
 #endif // BACKGROUND_HPP 