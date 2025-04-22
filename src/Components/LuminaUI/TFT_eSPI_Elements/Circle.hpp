/**
 * @file Circle.hpp
 * @brief UI element for rendering perfect circles using the Ellipse base class.
 *
 * Inherits from Ellipse and enforces equal radii to draw true circles
 * clipped to the element's viewport.
 */

 #ifndef CIRCLE_HPP
 #define CIRCLE_HPP
 
 #include "TFT_eSPI_Elements.hpp"  ///< Global TFT and sprite instances
 #include "Ellipse.hpp"           ///< Base class for ellipse shapes
 #include <TFT_eSPI.h>
 
 /**
  * @class Circle
  * @brief Renders a circle by constraining the Ellipse element to a square viewport.
  *
  * The Circle element calculates the maximum possible diameter that fits
  * within its assigned viewport and uses Ellipse drawing routines to render a circle.
  * Background fill and border options are inherited.
  */
 class Circle : public Ellipse {
 public:
     /**
      * @brief Construct a Circle element with styling options.
      *
      * @param element       Base Element defining origin, offset, and scale.
      * @param fillColor     Fill color for the circle interior (default TFT_WHITE).
      * @param borderColor   Color for the circle border (default TFT_BLACK).
      * @param renderFill    Enable interior fill drawing (default true).
      * @param renderBorder  Enable border outline drawing (default false).
      */
     Circle(
         const Element& element,
         uint32_t fillColor = TFT_WHITE,
         uint32_t borderColor = TFT_BLACK,
         bool renderFill = true,
         bool renderBorder = false)
       : Ellipse(element, fillColor, borderColor, renderFill, renderBorder) {}
 
     /**
      * @brief Render the circle within the provided viewport.
      *
      * Overrides Ellipse::render by adjusting the viewport to a square
      * having sides equal to the smaller dimension of the original viewport.
      * The resulting square viewport is passed to Ellipse to draw a circle.
      *
      * @param viewport  Clipping and offset rectangle for rendering.
      * @return The square Viewport used for circle rendering.
      */
     Viewport render(const Viewport& viewport) override {
         // Clip to this element's viewport without altering render flag
         Viewport circleVP = Element::render(viewport);
         // Determine maximum diameter that fits
         int diameter = min(circleVP.scale.x, circleVP.scale.y);
         // Center the circle by adjusting viewport scale
         circleVP.scale = ivec2(diameter, diameter);
         // Delegate drawing to Ellipse (which will draw a circle when radii equal)
         return Ellipse::render(circleVP);
     }
 };
 
 #endif // CIRCLE_HPP 