/**
 * @file Line.hpp
 * @brief UI element for drawing horizontal or vertical lines within a viewport.
 *
 * Inherits from Element and uses TFT_eSPI fast line draw functions to
 * render 1-pixel thick horizontal or vertical lines clipped to its viewport.
 */

 #ifndef LINE_HPP
 #define LINE_HPP
 
 #include "TFT_eSPI_Elements.hpp"
 
 /**
  * @class Line
  * @brief Renders a single-pixel line, either horizontal or vertical.
  *
  * The Line element draws either a horizontal or vertical line of specified length and color.
  * It automatically sets its scale based on orientation and provided length if scale was zero.
  */
 class Line : public Element {
 public:
     /** Line orientation types */
     enum Orientation { HORIZONTAL, VERTICAL };
 
     Orientation orientation; ///< Line direction
     uint32_t color;          ///< Line color (RGB565)
 
     /**
      * @brief Constructs a Line element based on an existing Element.
      *
      * @param element   Base Element defining origin and viewport.
      * @param orient    Orientation (HORIZONTAL or VERTICAL).
      * @param lineColor Color of the line (default black).
      * @param length    Length of the line in pixels; if zero, uses Element scale.
      */
     Line(
         const Element& element,
         Orientation orient,
         uint32_t lineColor,
         int length = 0)
       : Element(element), orientation(orient), color(lineColor) {
         // Determine default length if not specified
         int len = length;
         if (len == 0) {
             len = (orientation == HORIZONTAL) ? scale.x : scale.y;
         }
         // Set element scale based on orientation
         if (orientation == HORIZONTAL) {
             scale = ivec2(len, 1);
         } else {
             scale = ivec2(1, len);
         }
     }
 
     /**
      * @brief Render the line within the provided viewport.
      *
      * Clips to the element's viewport then draws a fast horizontal or vertical line
      * at the viewport's position with the configured length and color. Restores viewport afterward.
      *
      * @param viewport Clipping and offset rectangle for rendering.
      * @return The viewport used for this Line element.
      */
     Viewport render(const Viewport &viewport) override {
         Viewport lineVP = Element::render(viewport);
         // Apply clipping to viewport
         LuminaUI::tft_instance->setViewport(
             lineVP.position.x,
             lineVP.position.y,
             lineVP.scale.x,
             lineVP.scale.y);
         // Draw the line
         if (orientation == HORIZONTAL) {
             LuminaUI::tft_instance->drawFastHLine(
                 0, 0,
                 lineVP.scale.x,
                 color);
         } else {
             LuminaUI::tft_instance->drawFastVLine(
                 0, 0,
                 lineVP.scale.y,
                 color);
         }
         // Restore full viewport
         LuminaUI::tft_instance->resetViewport();
         return lineVP;
     }
 };
 
 #endif // LINE_HPP