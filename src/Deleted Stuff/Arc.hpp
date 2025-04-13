#ifndef ARC_HPP
#define ARC_HPP

#include "TFT_eSPI_Elements.hpp"
#include <TFT_eSPI.h>

class Arc : public Element
{
public:
    uint32_t foregroundColor; // Color of the arc
    uint32_t backgroundColor; // Background color
    int startAngle;   // Start angle in degrees [0..360]
    int sweepAngle;   // Sweep (arc) angle in degrees
    int thickness;    // Desired arc border thickness in pixels (arc thickness = r - ir + 1)
    bool roundEnds;   // Whether to round the arc's ends

    // Constructor with default options including roundEnds.
    Arc(const Element& element, 
        uint32_t foregroundColor = TFT_WHITE, 
        uint32_t backgroundColor = TFT_BLACK,
        int startAngle = 0, 
        int sweepAngle = 90,
        int thickness = 2,
        bool roundEnds = false)
      : Element(element), foregroundColor(foregroundColor), backgroundColor(backgroundColor),
        startAngle(startAngle), sweepAngle(sweepAngle), thickness(thickness),
        roundEnds(roundEnds)
    {}

    Viewport render(const Viewport &viewport) override {
        // Render the parent element and set the viewport for this element.
        Viewport arcViewport = Element::render(viewport);
        LuminaUI::tft_instance->setViewport(
            arcViewport.position.x, 
            arcViewport.position.y, 
            arcViewport.scale.x, 
            arcViewport.scale.y
        );

        // Compute the center of the viewport and determine the outer radius using the smaller dimension.
        int centerX = arcViewport.scale.x / 2;
        int centerY = arcViewport.scale.y / 2;
        int radius = (arcViewport.scale.x < arcViewport.scale.y ? arcViewport.scale.x : arcViewport.scale.y) / 2;

        // Compute the end angle, clamping to 360 if necessary.
        int endAngle = startAngle + sweepAngle;
        if (endAngle > 360)
            endAngle = 360;  // Clamp to 360; wrap-around arcs are not supported.

        // Draw the arc using the TFT_eSPI library.
        LuminaUI::tft_instance->drawSmoothArc(centerX, centerY, radius, radius-thickness, startAngle, endAngle, foregroundColor, backgroundColor, roundEnds);

        // Reset the viewport back to the full display.
        LuminaUI::tft_instance->resetViewport();
        return arcViewport;
    }
};

#endif // ARC_HPP