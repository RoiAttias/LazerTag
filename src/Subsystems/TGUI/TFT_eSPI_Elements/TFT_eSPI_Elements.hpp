#ifndef TFT_ESPI_ELEMENTS_HPP
#define TFT_ESPI_ELEMENTS_HPP

#include <TFT_eSPI.h>

namespace TGUI
{
TFT_eSPI * tft_instance = nullptr;
TFT_eSprite * img_instance = nullptr;

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

void drawTriangles(TFT_eSPI *tftptr, ivec2 * vertices, uint vertexCount, uint triangles[][3], uint triangleCount, uint32_t color)
{
    ivec2 triangleVertices[3];
    for (uint i = 0; i < triangleCount; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (triangles[i][j] >= vertexCount)
            {
                triangles[i][j] = 0;
            }
            triangleVertices[j] = vertices[triangles[i][j]];
        }
        tftptr->drawTriangle(triangleVertices[0].x, triangleVertices[0].y,
                                   triangleVertices[1].x, triangleVertices[1].y,
                                   triangleVertices[2].x, triangleVertices[2].y,
                                   color);
    }
}

void fillPolygon(iTFT_eSPI *tftptr, ivec2 * vertices, uint vertexCount, uint triangles[][3], uint triangleCount, uint32_t color)
{
    ivec2 triangleVertices[3];
    for (int i = 0; i < triangleCount; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (triangles[i][j] >= vertexCount)
            {
                triangles[i][j] = 0;
            }
            triangleVertices[j] = vertices[triangles[i][j]];
        }
        tftptr->fillTriangle(triangleVertices[0].x, triangleVertices[0].y,
                                    triangleVertices[1].x, triangleVertices[1].y,
                                    triangleVertices[2].x, triangleVertices[2].y,
                                    color);
    }
}
}; // namespace TGUI

// Touch Drivers
#include "Touch_XPT2046.hpp"

// Elements
/*
rectrangle (round)
text box (text, font, size, color, onlytext/withbackground/box)
button
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