/**
 * @file Polygon.hpp
 * @brief UI element for rendering filled and/or outlined polygons defined by vertices and triangle indices.
 *
 * Inherits from Element and uses HyperList to manage vertex and triangle data.
 * Provides coordinate calculation (scaling and translation) based on Viewport,
 * and renders using TFT_eSPI_Elements fillPolygon and drawTriangles functions.
 */

 #ifndef POLYGON_HPP
 #define POLYGON_HPP
 
 #include "TFT_eSPI_Elements.hpp"    ///< Drawing helpers for TFT_eSPI
 #include "Utilities/HyperList.hpp"  ///< Dynamic list container
 #include <TFT_eSPI.h>
 
 /**
  * @brief Example polygon vertex list for a single triangle in normalized coordinates.
  */
 static vec2 polygon_vertices_example[] = {
     {0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}
 };
 
 /**
  * @brief Number of vertices in `polygon_vertices_example`.
  */
 static const uint polygon_vertices_example_count = sizeof(polygon_vertices_example) / sizeof(vec2);
 
 /**
  * @brief Example triangle index list for the example polygon (3 indices per triangle).
  */
 static uint polygon_triangles_example[] = {
     0, 1, 2
 };
 
 /**
  * @brief Number of indices in `polygon_triangles_example`.
  */
 static const uint polygon_triangles_example_count = sizeof(polygon_triangles_example) / sizeof(uint);
 
 
 /**
  * @class Polygon
  * @brief Draws arbitrary polygons by triangulation within a clipped Viewport.
  *
  * Members:
  *  - vertices: Relative float coordinates of polygon points (0.0-1.0 range)
  *  - triangles: Indices into vertices list, grouped by threes
  *  - fillColor: Color for filled interior
  *  - lineColor: Color for triangle outlines
  *  - renderFill: Flag to enable filling
  *  - renderLines: Flag to enable drawing edges
  *  - polygon_position: Relative offset (0.0-1.0) within Viewport
  *  - polygon_scale: Relative scaling (0.0-1.0) within Viewport
  */
 class Polygon : public Element {
 public:
     HyperList<vec2> vertices;        ///< Relative vertex coordinates
     HyperList<ivec2> verticesCalculated; ///< Absolute pixel positions after transform
     HyperList<uint> triangles;       ///< Triangle index lists (3 indices per triangle)
 
     vec2 polygon_position;           ///< Relative translation within Viewport
     vec2 polygon_scale;              ///< Relative scaling within Viewport
 
     uint32_t fillColor;              ///< Fill color (RGB565)
     uint32_t lineColor;              ///< Line color for outlines (RGB565)
     bool renderFill;                 ///< Enable interior fill
     bool renderLines;                ///< Enable outline drawing
 
     /**
      * @brief Constructs a Polygon element with vertex and triangle data.
      *
      * @param element         Base Element defining origin, offset, and scale
      * @param vertices        Array of vec2 defining relative polygon points
      * @param vertexCount     Number of vertices in the array
      * @param triangles       Array of uint indices (3 per triangle)
      * @param triangleCount   Number of indices in triangles array
      * @param fillColor       Color for interior fill (default white)
      * @param lineColor       Color for edges (default black)
      * @param renderFill      Enable filling (default true)
      * @param renderLines     Enable edge drawing (default false)
      * @param polygon_position Relative offset inside Viewport (default 0,0)
      * @param polygon_scale   Relative scale inside Viewport (default 1,1)
      */
     Polygon(
         const Element& element,
         vec2* vertices = polygon_vertices_example,
         uint vertexCount = polygon_vertices_example_count,
         uint* triangles = polygon_triangles_example,
         uint triangleCount = polygon_triangles_example_count,
         uint32_t fillColor = TFT_WHITE,
         uint32_t lineColor = TFT_BLACK,
         bool renderFill = true,
         bool renderLines = false,
         vec2 polygon_position = vec2(0.0f, 0.0f),
         vec2 polygon_scale = vec2(1.0f, 1.0f))
       : Element(element)
       , fillColor(fillColor)
       , lineColor(lineColor)
       , renderFill(renderFill)
       , renderLines(renderLines)
       , polygon_position(polygon_position)
       , polygon_scale(polygon_scale) {
         this->vertices.addFromArray(vertices, vertexCount);
         this->triangles.addFromArray(triangles, triangleCount);
     }
 
     /**
      * @brief Transform relative vertices into absolute pixel positions.
      *
      * Scales and translates each vertex based on polygon_position and polygon_scale,
      * then maps to pixel coordinates within the given Viewport.
      * @param viewport Clipping region for rendering.
      */
     void calculateVertices(const Viewport& viewport) {
         verticesCalculated.clear();
         for (int i = 0; i < vertices.size(); ++i) {
             vec2 v = vertices.get(i);
             // Apply relative scaling and translation
             v = v.multiply(polygon_scale) + polygon_position;
             // Map into viewport coordinates
             vec2 abs = vec2(viewport.position) + v.multiply(viewport.scale);
             // Convert to integer pixel positions
             ivec2 iv = ivec2((int)abs.x, (int)abs.y);
             verticesCalculated.addend(iv);
         }
     }
 
     /**
      * @brief Render the polygon within a clipped viewport.
      *
      * Clips to element area, calculates vertices, allocates buffers,
      * and calls fillPolygon and/or drawTriangles. Frees buffers and restores viewport.
      *
      * @param viewport Clipping and offset rectangle.
      * @return The Viewport used for rendering.
      */
     Viewport render(const Viewport& viewport) override {
         Viewport vp = Element::render(viewport);
         LuminaUI::tft_instance->setViewport(
             vp.position.x, vp.position.y,
             vp.scale.x, vp.scale.y);
         calculateVertices(vp);
         if (verticesCalculated.size() == 0 || triangles.size() == 0) {
             LuminaUI::tft_instance->resetViewport();
             return vp;
         }
         // Allocate buffers for native draw functions
         ivec2* vertBuf = (ivec2*)malloc(sizeof(ivec2) * verticesCalculated.size());
         uint* triBuf = (uint*)malloc(sizeof(uint) * triangles.size());
         verticesCalculated.storeToArray(vertBuf, verticesCalculated.size());
         triangles.storeToArray(triBuf, triangles.size());
         // Draw fill and/or outlines
         if (renderFill) {
             LuminaUI::fillPolygon(LuminaUI::tft_instance,
                 vertBuf, verticesCalculated.size(),
                 triBuf, triangles.size(), fillColor);
         }
         if (renderLines) {
             LuminaUI::drawTriangles(LuminaUI::tft_instance,
                 vertBuf, verticesCalculated.size(),
                 triBuf, triangles.size(), lineColor);
         }
         free(vertBuf);
         free(triBuf);
         LuminaUI::tft_instance->resetViewport();
         return vp;
     }
 };
 
 #endif // POLYGON_HPP 