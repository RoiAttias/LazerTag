/**
 * @file HpBar.hpp
 * @brief Defines the HpBar GUI element for displaying a health bar in the Manager UI.
 *
 * The HpBar element visualizes a numeric value (e.g., player health) as a colored bar
 * with percentage-based fill and accompanying text "current/max". It inherits from Element
 * and composes a border rectangle, a fill rectangle, and a text label. The fill color
 * transitions from green to red based on the percentage.
 */

 #ifndef HPBAR_HPP
 #define HPBAR_HPP
 
 #include "MANAGER/GUI_Manager/GUI_Manager.hpp"
 #include "Utilities/MoreMath.hpp"
 
 /**
  * @class HpBar
  * @brief Renders a health bar with dynamic fill and color based on current value.
  *
  * The HpBar element:
  * - Draws an outer border rectangle.
  * - Draws an inner fill rectangle whose width reflects the current value percentage.
  * - Displays "value/maxValue" centered over the bar.
  * - Computes fill color via HSV hue interpolation (green to red).
  * - Computes text color as an inverse brightness for readability.
  */
 class HpBar : public Element
 {
     Rectangle border;   ///< Outer border rectangle.
     Rectangle fill;     ///< Inner fill rectangle.
     Text      text;     ///< Overlay text showing "value/maxValue".
 
     int   value    = 100; ///< Current numeric value.
     int   maxValue = 100; ///< Maximum value (percent denominator).
     float percent;        ///< value / maxValue ratio.
     ivec2 fillScale;      ///< Dimensions for fill rectangle.
     String textContent;   ///< "value/maxValue" string.
 
     uint32_t color1;      ///< Fill and border color (HSV-based).
     uint32_t color2;      ///< Text color (inverse brightness).
 
     ivec2 margin = {5, 5}; ///< Padding inside the border.
 
 public:
     /**
      * @brief Construct a new HpBar element.
      * @param element Base Element specifying origin and scale.
      *
      * Initializes sub-elements (border, fill, text) with default colors
      * and calls setValue to apply the initial state.
      */
     HpBar(const Element &element)
         : Element(element),
           border(Element(), TFT_WHITE, TFT_WHITE, false, true),
           fill(Element(),   TFT_WHITE, TFT_WHITE, true,  false),
           text(Element(),   "0/0",      TFT_BLACK, 1, MC_DATUM, 0.0f, &FreeMonoBold12pt7b)
     {
         setValue(value);
     }
 
     /**
      * @brief Update the current value and trigger a re-render.
      * @param newValue New numeric value to display.
      *
      * Stores the value and marks the element for redraw.
      */
     void setValue(int newValue)
     {
         value = newValue;
         Element::callRender();
     }
 
     /**
      * @brief Render the health bar into its viewport.
      * @param viewport Parent clipping region.
      * @return The viewport actually used by this element.
      *
      * Steps:
      * 1. Compute `percent = float(value)/maxValue`.
      * 2. Compute fillScale = overall scale minus margins.
      * 3. Prepare textContent = "value/maxValue".
      * 4. Compute hue from percent (green→red) and convert to RGB.
      * 5. Compute text color as inverse brightness.
      * 6. Position and style border, fill, and text sub-elements.
      * 7. Call base `Element::render` to clamp viewport.
      * 8. Render border, fill, then text in sequence.
      */
     Viewport render(const Viewport &viewport) override
     {
         // Determine inner fill area and percentage
         fillScale = scale - (margin * 2);
         percent   = float(value) / float(maxValue);
         textContent = String(value) + "/" + String(maxValue);
 
         // Compute fill color: hue from 0.0 (red) to 0.333 (green)
         float hue = max(percent - 0.2f, 0.0f) / 0.8f * 0.333f;
         byte  r, g, b;
         hueToRgb(hue, &r, &g, &b);
         color1 = LuminaUI::tft_instance->color565(r, g, b);
 
         // Compute text color as inverse brightness
         byte v = 255 - byte(percent * 255);
         color2 = LuminaUI::tft_instance->color565(v, v, v);
 
         // Configure border rectangle
         border.origin      = origin;
         border.scale       = scale;
         border.borderColor = color1;
         border.fillColor   = color1;
 
         // Configure fill rectangle
         fill.origin        = origin + margin;
         fill.scale         = ivec2(int(fillScale.x * percent), fillScale.y);
         fill.borderColor   = color1;
         fill.fillColor     = color1;
 
         // Configure text label
         text.origin      = origin + margin;
         text.scale       = fillScale;
         text.content     = textContent;
         text.textColor   = color2;
 
         // Render sub-elements
         Viewport vp = Element::render(viewport);
         border.render(vp);
         fill.render(vp);
         text.render(vp);
         return vp;
     }
 };
 
 #endif // HPBAR_HPP 