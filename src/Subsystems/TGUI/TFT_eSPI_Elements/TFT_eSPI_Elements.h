#ifndef TFT_ESPI_ELEMENTS_HPP
#define TFT_ESPI_ELEMENTS_HPP

#include "Subsystems/TGUI/TGUI.hpp"
#include <TFT_eSPI.h>

namespace TGUI
{
    TFT_eSPI * tft_instance;
    TFT_eSprite * img_instance;

    void push_TFT_eSprite()
    {
        img_instance->pushSprite(0,0);
    }

    void drawPolygon(TFT_eSPI *tftptr, ivec2 * vertices, uint vertexCount, uint32_t color)
    {
        ivec2 p1,p2;
        for (uint i = 0; i < vertexCount; i++)
        {
            p1 = ivec2(vertices[i].x, vertices[i].y);
            p2 = ivec2(vertices[(i + 1) % vertexCount].x, vertices[(i + 1) % vertexCount].y);
            tftptr->drawLine(p1.x, p1.y, p2.x, p2.y, color);
        }
    }

    void drawTriangles(TFT_eSPI *tftptr, ivec2 * vertices, uint vertexCount, uint *triangles, int triangleCount, uint32_t color)
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
            tftptr->drawTriangle(triangleVertices[0].x, triangleVertices[0].y,
                                    triangleVertices[1].x, triangleVertices[1].y,
                                    triangleVertices[2].x, triangleVertices[2].y,
                                    color);
        }
    }

    void fillPolygon(TFT_eSPI *tftptr, ivec2 * vertices, uint vertexCount, uint *triangles, uint triangleCount, uint32_t color)
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
            tftptr->fillTriangle(triangleVertices[0].x, triangleVertices[0].y,
                                        triangleVertices[1].x, triangleVertices[1].y,
                                        triangleVertices[2].x, triangleVertices[2].y,
                                        color);
        }
    }
}; // namespace TGUI

// Fonts
#include "Free_Fonts.h"

// Touch Drivers
#include "Touch_XPT2046.hpp"

// Base Elements
#include "Rectangle.hpp"
#include "Text.hpp"
#include "Ellipse.hpp"
#include "Background.hpp"

// Layout Elements
//#include "Table.hpp" - not ready

// Combined Elements
//#include "Textbox.hpp" - not ready

// Overriden Elements
//#include "Button.hpp"
#include "Circle.hpp"
/*
shape
image
polygon
circle
line
devider(horizonal, vertical)
progress bar
table(pointer of grid, pointer to list of grids)
*/
//#include "Shape.hpp"

#endif // TFT_ESPI_ELEMENTS_HPP