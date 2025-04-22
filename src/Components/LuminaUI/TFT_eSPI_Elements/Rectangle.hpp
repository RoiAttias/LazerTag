/**
 * @file Rectangle.hpp
 * @brief UI element to draw a filled and/or bordered rectangle with optional rounded corners.
 *
 * Inherits from Element and leverages TFT_eSPI viewport functions to render
 * a rectangle within its assigned clipping region.
 */

 #ifndef RECTANGLE_HPP
 #define RECTANGLE_HPP
 
 #include "TFT_eSPI_Elements.hpp"
 #include <TFT_eSPI.h>
 
 /**
  * @class Rectangle
  * @brief Draws a rectangular UI element with fill and border options.
  *
  * Supports filled or bordered rectangles, with optional corner radius for rounded corners.
  * Uses the global TFT_eSPI sprite instance and viewport clipping to render.
  */
 class Rectangle : public Element {
 public:
     uint32_t fillColor;   ///< Color used to fill the rectangle (16-bit RGB565)
     uint32_t borderColor; ///< Color used for rectangle border (16-bit RGB565)
     bool renderFill;      ///< Whether to draw the filled interior
     bool renderBorder;    ///< Whether to draw the border outline
     int cornerRadius;     ///< Radius for rounded corners (0 = sharp corners)
 
     /**
      * @brief Construct a Rectangle element based on an existing Element.
      *
      * @param element      Base Element defining origin, offset, scale, and visibility
      * @param fillColor    Fill color for the rectangle (default TFT_WHITE)
      * @param borderColor  Border color for the rectangle (default TFT_BLACK)
      * @param renderFill   Set to true to draw filled interior
      * @param renderBorder Set to true to draw border outline
      * @param cornerRadius Corner radius in pixels for rounded edges
      */
     Rectangle(
         Element element,
         uint32_t fillColor = TFT_WHITE,
         uint32_t borderColor = TFT_BLACK,
         bool renderFill = true,
         bool renderBorder = false,
         int cornerRadius = 0)
       : Element(element)
       , fillColor(fillColor)
       , borderColor(borderColor)
       , renderFill(renderFill)
       , renderBorder(renderBorder)
       , cornerRadius(cornerRadius) {}
 
     /**
      * @brief Render the rectangle within the provided viewport.
      *
      * Applies clipping via TFT_eSPI::setViewport and draws fill and/or border
      * with optional rounded corners. Restores full viewport after drawing.
      *
      * @param viewport Clipping region for rendering.
      * @return The viewport used for the rectangle (clipped by parent).
      */
     Viewport render(const Viewport &viewport) override {
         // Clip to this element's viewport
         Viewport rectVP = Element::render(viewport);
         LuminaUI::tft_instance->setViewport(
             rectVP.position.x,
             rectVP.position.y,
             rectVP.scale.x,
             rectVP.scale.y);
 
         // Determine effective corner radius (max half of width/height)
         int maxR = min(rectVP.scale.x, rectVP.scale.y) / 2;
         int r = min(cornerRadius, maxR);
 
         // Draw filled interior if requested
         if (renderFill) {
             if (r > 0) {
                 LuminaUI::tft_instance->fillRoundRect(
                     0, 0,
                     rectVP.scale.x,
                     rectVP.scale.y,
                     r,
                     fillColor);
             } else {
                 LuminaUI::tft_instance->fillRect(
                     0, 0,
                     rectVP.scale.x,
                     rectVP.scale.y,
                     fillColor);
             }
         }
 
         // Draw border if requested
         if (renderBorder) {
             if (r > 0) {
                 LuminaUI::tft_instance->drawRoundRect(
                     0, 0,
                     rectVP.scale.x,
                     rectVP.scale.y,
                     r,
                     borderColor);
             } else {
                 LuminaUI::tft_instance->drawRect(
                     0, 0,
                     rectVP.scale.x,
                     rectVP.scale.y,
                     borderColor);
             }
         }
 
         // Restore full viewport for subsequent draws
         LuminaUI::tft_instance->resetViewport();
         return rectVP;
     }
 };
 
 #endif // RECTANGLE_HPP 