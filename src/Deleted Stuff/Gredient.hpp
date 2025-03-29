#ifndef GRADIENT_HPP
#define GRADIENT_HPP

#include "MANAGER/GUI_Manager/GUI_Manager.hpp"
#include "Utilities/MoreMath.hpp"

class Gradient : public Element
{
public:
    // Colors stored as [R, G, B]
    uint8_t colorLeft[3];
    uint8_t colorRight[3];

    // Constructor: takes the base Element and two 3-element color arrays.
    Gradient(Element element, const uint8_t _colorLeft[3], const uint8_t _colorRight[3])
      : Element(element)
    {
        memcpy(colorLeft, _colorLeft, 3);
        memcpy(colorRight, _colorRight, 3);
    }

    // Render method draws a horizontal linear gradient (left: colorLeft, right: colorRight)
    Viewport render(const Viewport &viewport) override {
        Viewport gradViewport = Element::render(viewport);
        LuminaUI::tft_instance->setViewport(gradViewport.position.x, gradViewport.position.y, gradViewport.scale.x, gradViewport.scale.y);
        
        // Iterate over each vertical line in the element's width.
        for (int x = 0; x < gradViewport.scale.x; x++) {
            // Calculate the interpolation factor (0.0 at left edge, 1.0 at right edge)
            float factor = (float)(x) / (gradViewport.scale.x - 1);

            // Convert the blended RGB color to TFT's 16-bit (565) format.
            uint16_t col = LuminaUI::tft_instance->color565(r, g, b);

            // Draw a vertical line at the current x position.
            LuminaUI::tft_instance->drawFastVLine(x, 0, gradViewport.scale.y, col);
        }
        LuminaUI::tft_instance->resetViewport();
        return gradViewport;
    }
};

#endif // GRADIENT_HPP
