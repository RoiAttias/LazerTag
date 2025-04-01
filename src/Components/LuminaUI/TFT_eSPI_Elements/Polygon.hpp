#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "TFT_eSPI_Elements.hpp"
#include "Utilities/HyperList.hpp"
#include <TFT_eSPI.h>

vec2 polygon_vertices_example[] = {
    {0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}
};

uint polygon_vertices_example_count = sizeof(polygon_vertices_example) / sizeof(vec2);

uint polygon_triangles_example[] = {
    0, 1, 2
};

uint polygon_triangles_example_count = sizeof(polygon_triangles_example) / sizeof(uint);

class Polygon : public Element
{
public:
    HyperList<vec2> vertices;
    HyperList<ivec2> verticesCalculated;
    HyperList<uint> triangles;

    vec2 polygon_position;
    vec2 polygon_scale;

    uint32_t fillColor;
    uint32_t lineColor;
    bool renderFill;
    bool renderLines;

    Polygon(const Element &element, 
        vec2 * vertices = polygon_vertices_example,
        uint vertexCount = polygon_vertices_example_count,
        uint * triangles = polygon_triangles_example,
        uint triangleCount = polygon_triangles_example_count,
        uint32_t fillColor = TFT_WHITE, uint32_t lineColor = TFT_BLACK,
        bool renderFill = true, bool renderLines = false,
        vec2 polygon_position = vec2(0.0f, 0.0f), vec2 polygon_scale = vec2(1.0f, 1.0f))
        : Element(element), fillColor(fillColor), lineColor(lineColor),
        renderFill(renderFill), renderLines(renderLines),
        polygon_position(polygon_position), polygon_scale(polygon_scale)
    {
        this->vertices.addFromArray(vertices, vertexCount);
        this->triangles.addFromArray(triangles, triangleCount);
    }

    void calculateVertices(const Viewport &viewport)
    {
        verticesCalculated.clear();
        for (int i = 0; i < vertices.size(); i++)
        {
            vec2 vertex = vertices[i];
            vertex = vertex.multiply(polygon_scale) + polygon_position;
            vertex = vec2(viewport.position) + vertex.multiply(viewport.scale);
            ivec2 calculatedVertex = ivec2((int)vertex.x, (int)vertex.y);
            verticesCalculated.addend(calculatedVertex);
        }
    }

    Viewport render(const Viewport &viewport) override {
        Viewport vp = Element::render(viewport);
        LuminaUI::tft_instance->setViewport(vp.position.x, vp.position.y, vp.scale.x, vp.scale.y);
        calculateVertices(vp);
        if (verticesCalculated.size() == 0 || triangles.size() == 0)
        {
            return vp;
        }
        ivec2 * verticesBuffer = (ivec2 *)malloc(sizeof(ivec2) * verticesCalculated.size());
        uint * trianglesBuffer = (uint *)malloc(sizeof(uint) * triangles.size());
        verticesCalculated.storeToArray(verticesBuffer, verticesCalculated.size());
        triangles.storeToArray(trianglesBuffer, triangles.size());
        if (renderFill)
        {
            LuminaUI::fillPolygon(LuminaUI::tft_instance, verticesBuffer, verticesCalculated.size(),
                trianglesBuffer, triangles.size(), fillColor);
        }
        if (renderLines)
        {
            LuminaUI::drawTriangles(LuminaUI::tft_instance, verticesBuffer, verticesCalculated.size(),
                trianglesBuffer, triangles.size(), lineColor);
        }
        free(verticesBuffer);
        free(trianglesBuffer);
        LuminaUI::tft_instance->resetViewport();
        return vp;
    }
};


#endif // POLYGON_HPP