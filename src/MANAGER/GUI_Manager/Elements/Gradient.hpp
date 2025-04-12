#ifndef GRADIENT_HPP
#define GRADIENT_HPP

#include "MANAGER/GUI_Manager/GUI_Manager.hpp"
#include "Utilities/MoreMath.hpp"

class Gradient : public Element
{
public:
    // Color arrays for the gradient
    uint8_t colorLeft[3] = {0, 0, 255}; // Blue
    uint8_t colorRight[3] = {255, 0, 0}; // Red

    // Constructor
    Gradient(Element element = Element()) 
        : Element(element) {}

    // Render method draws a horizontal linear gradient (left: colorLeft, right: colorRight)
    Viewport render(const Viewport &viewport) override {
        Viewport gradViewport = Element::render(viewport);
        LuminaUI::tft_instance->setViewport(gradViewport.position.x, gradViewport.position.y, gradViewport.scale.x, gradViewport.scale.y);
        
        // Iterate over each vertical line in the element's width.
        for (int x = 0; x < gradViewport.scale.x; x++) {
            // Calculate the interpolation factor (0.0 at left edge, 1.0 at right edge)
            float factor = (float)(x) / (gradViewport.scale.x - 1);
            byte r = mix(factor, colorLeft[0], colorRight[0]);
            byte g = mix(factor, colorLeft[1], colorRight[1]);
            byte b = mix(factor, colorLeft[2], colorRight[2]);

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
