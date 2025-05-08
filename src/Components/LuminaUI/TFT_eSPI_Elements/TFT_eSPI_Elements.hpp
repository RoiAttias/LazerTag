#ifndef TFT_ESPI_ELEMENTS_HPP
#define TFT_ESPI_ELEMENTS_HPP

#include "Components/LuminaUI/LuminaUI.hpp"
#include <TFT_eSPI.h>

namespace LuminaUI
{
    TFT_eSPI * tft_instance;

    void drawTriangles(ivec2 * vertices, uint vertexCount, uint *triangles, int triangleCount, uint32_t color)
    {
        ivec2 triangleVertices[3];
        int selectedVertex;
        for (int i = 0; i < triangleCount; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                selectedVertex = triangles[i * 3 + j];
                if (selectedVertex >= vertexCount)
                {
                    selectedVertex = 0;
                }
                triangleVertices[j] = vertices[selectedVertex];
            }
            tft_instance->drawTriangle(triangleVertices[0].x, triangleVertices[0].y,
                                    triangleVertices[1].x, triangleVertices[1].y,
                                    triangleVertices[2].x, triangleVertices[2].y,
                                    color);
        }
    }

    void fillPolygon(ivec2 * vertices, uint vertexCount, uint *triangles, uint triangleCount, uint32_t color)
    {
        ivec2 triangleVertices[3];
        int selectedVertex;
        for (int i = 0; i < triangleCount; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                selectedVertex = triangles[i * 3 + j];
                if (selectedVertex >= vertexCount)
                {
                    selectedVertex = 0;
                }
                triangleVertices[j] = vertices[selectedVertex];
            }
            tft_instance->fillTriangle(triangleVertices[0].x, triangleVertices[0].y,
                                        triangleVertices[1].x, triangleVertices[1].y,
                                        triangleVertices[2].x, triangleVertices[2].y,
                                        color);
        }
    }
}; // namespace LuminaUI

// Fonts
#include "Free_Fonts.h"

// Touch Drivers
#include "Touch_XPT2046.hpp"

// Base Elements
#include "Rectangle.hpp"
#include "Text.hpp"
#include "Ellipse.hpp"
#include "Background.hpp"
#include "Line.hpp"
#include "Polygon.hpp"

// Combined Elements
#include "Textbox.hpp"

// Overriden Elements
#include "Button.hpp"
#include "Circle.hpp"

#endif // TFT_ESPI_ELEMENTS_HPP