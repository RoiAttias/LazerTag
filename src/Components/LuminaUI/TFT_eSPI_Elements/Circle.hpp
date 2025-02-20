#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "TFT_eSPI_Elements.hpp"
#include "Ellipse.hpp" // Include the header file for the Ellipse class
#include <TFT_eSPI.h>

class Circle : public Ellipse {
public:
    // Constructor for Circle: Calls Ellipse constructor but ensures equal radii for both axes
    Circle(const Element& element, uint32_t fillColor = TFT_WHITE, uint32_t borderColor = TFT_BLACK,
           bool renderFill = true, bool renderBorder = false)
        : Ellipse(element, fillColor, borderColor, renderFill, renderBorder) {}

    // Override the render method to enforce equal radii for a circle
    Viewport render(const Viewport& viewport) override {
        // Calculate the circle's viewport based on the element's position and size
        Viewport circleViewport = Element::render(viewport);

        // Calculate the diameter (smallest of width and height)
        int diameter = min(circleViewport.scale.x, circleViewport.scale.y);

        // Set the scale to the diameter for both axes
        circleViewport.scale = ivec2(diameter, diameter);

        // Pass the calculated circle viewport to the Ellipse render function
        return Ellipse::render(circleViewport);
    }
};

#endif // CIRCLE_HPP
