/**
 * @file Ellipse.hpp
 * @brief UI element to draw filled and/or bordered ellipses or circles within a viewport.
 *
 * Inherits from Element and uses TFT_eSPI viewport functions to render
 * ellipses with optional fill and border colors.
 */

 #ifndef ELLIPSE_HPP
 #define ELLIPSE_HPP
 
 #include "TFT_eSPI_Elements.hpp"
 #include <TFT_eSPI.h>
 
 /**
  * @class Ellipse
  * @brief Renders an ellipse or circle depending on aspect ratio, with fill and border options.
  *
  * The Ellipse class draws shapes clipped to its assigned Viewport, using
  * TFT_eSPI's fillEllipse/drawEllipse for non-circular shapes and
  * fillCircle/drawCircle when width and height are equal. It supports
  * configurable fill and border colors.
  */
 class Ellipse : public Element {
 public:
     uint32_t fillColor;   ///< Color used to fill the ellipse (RGB565)
     uint32_t borderColor; ///< Color used for the border outline (RGB565)
     bool renderFill;      ///< Flag to enable interior fill rendering
     bool renderBorder;    ///< Flag to enable border outline rendering
 
     /**
      * @brief Constructs an Ellipse element based on an existing Element.
      *
      * @param element       Base Element defining origin, offset, scale, and visibility
      * @param fillColor     Fill color for the ellipse (default TFT_WHITE)
      * @param borderColor   Border color for the ellipse outline (default TFT_BLACK)
      * @param renderFill    Enable drawing of the filled interior
      * @param renderBorder  Enable drawing of the border outline
      */
     Ellipse(
         const Element& element,
         uint32_t fillColor = TFT_WHITE,
         uint32_t borderColor = TFT_BLACK,
         bool renderFill = true,
         bool renderBorder = false)
       : Element(element)
       , fillColor(fillColor)
       , borderColor(borderColor)
       , renderFill(renderFill)
       , renderBorder(renderBorder) {}
 
     /**
      * @brief Render the ellipse within the provided viewport.
      *
      * Clips drawing to the element's Viewport using setViewport. Calculates
      * center and radii based on viewport dimensions. Chooses circle routines
      * when radii are equal, otherwise uses ellipse routines. Resets viewport after.
      *
      * @param viewport Clipping and offset rectangle for rendering.
      * @return The viewport used for this Ellipse element.
      */
     Viewport render(const Viewport &viewport) override {
         // Clip to this element's viewport
         Viewport ellipseVP = Element::render(viewport);
         LuminaUI::tft_instance->setViewport(
             ellipseVP.position.x,
             ellipseVP.position.y,
             ellipseVP.scale.x,
             ellipseVP.scale.y);
 
         // Compute center and radii
         int cx = ellipseVP.scale.x / 2;
         int cy = ellipseVP.scale.y / 2;
         int rx = ellipseVP.scale.x / 2;
         int ry = ellipseVP.scale.y / 2;
 
         // Draw filled shape if requested
         if (renderFill) {
             if (rx != ry) {
                 LuminaUI::tft_instance->fillEllipse(cx, cy, rx, ry, fillColor);
             } else {
                 LuminaUI::tft_instance->fillCircle(cx, cy, rx, fillColor);
             }
         }
 
         // Draw border if requested
         if (renderBorder) {
             if (rx != ry) {
                 LuminaUI::tft_instance->drawEllipse(cx, cy, rx, ry, borderColor);
             } else {
                 LuminaUI::tft_instance->drawCircle(cx, cy, rx, borderColor);
             }
         }
 
         // Restore full viewport
         LuminaUI::tft_instance->resetViewport();
         return ellipseVP;
     }
 };
 
 #endif // ELLIPSE_HPP