/**
 * @file Gradient.hpp
 * @brief Defines a horizontal gradient background element for the Manager UI.
 *
 * The Gradient element renders a smooth left-to-right linear color blend between
 * two RGB endpoints. It inherits from Element and draws vertical lines across its
 * width, interpolating the color at each x-coordinate.
 */

 #ifndef GRADIENT_HPP
 #define GRADIENT_HPP
 
 #include "MANAGER/GUI_Manager/GUI_Manager.hpp"
 #include "Utilities/MoreMath.hpp"
 
 /**
  * @class Gradient
  * @brief Renders a full-size horizontal gradient within its viewport.
  *
  * The Gradient element linearly interpolates between two RGB colors,
  * `colorLeft` at the left edge and `colorRight` at the right edge, by
  * drawing one vertical line of blended color per column. It uses the
  * `mix` utility for per-channel interpolation and the TFT_eSPI `color565`
  * conversion for 16-bit display output.
  */
 class Gradient : public Element
 {
 public:
     /**
      * @brief RGB color at the left edge of the gradient.
      * Stored as three bytes: { R, G, B }.
      */
     uint8_t colorLeft[3] = {0,   0, 255};  
 
     /**
      * @brief RGB color at the right edge of the gradient.
      * Stored as three bytes: { R, G, B }.
      */
     uint8_t colorRight[3] = {255, 0,   0};  
 
     /**
      * @brief Construct a new Gradient element.
      * @param element Base element defining position and size; defaults to auto.
      */
     Gradient(Element element = Element()) 
         : Element(element) {}
 
     /**
      * @brief Render the gradient into its viewport.
      * @param viewport The parent clipping region.
      * @return The viewport occupied by this element.
      *
      * This method:
      * 1. Calls the base `render` to obtain and clamp its own viewport.
      * 2. Sets the TFT viewport accordingly.
      * 3. For each x from 0 to width-1:
      *    - Computes `factor = x / (width-1)` in [0,1].
      *    - Blends each RGB channel linearly: `mix(factor, colorLeft[i], colorRight[i])`.
      *    - Converts to 16-bit color and draws a vertical line of that color.
      * 4. Resets the TFT viewport.
      */
     Viewport render(const Viewport &viewport) override {
         // Base class handles _shouldRender flag and viewport clamping
         Viewport gradViewport = Element::render(viewport);
 
         // Apply the clipped viewport to the TFT
         LuminaUI::tft_instance->setViewport(
             gradViewport.position.x,
             gradViewport.position.y,
             gradViewport.scale.x,
             gradViewport.scale.y
         );
 
         // Draw a vertical line for each column
         for (int x = 0; x < gradViewport.scale.x; x++) {
             float factor = float(x) / float(gradViewport.scale.x - 1);
 
             // Interpolate each channel
             byte r = mix(factor, colorLeft[0],  colorRight[0]);
             byte g = mix(factor, colorLeft[1],  colorRight[1]);
             byte b = mix(factor, colorLeft[2],  colorRight[2]);
 
             // Convert to 16-bit and draw
             uint16_t col = LuminaUI::tft_instance->color565(r, g, b);
             LuminaUI::tft_instance->drawFastVLine(x, 0, gradViewport.scale.y, col);
         }
 
         // Restore full-screen drawing
         LuminaUI::tft_instance->resetViewport();
         return gradViewport;
     }
 };
 
 #endif // GRADIENT_HPP 