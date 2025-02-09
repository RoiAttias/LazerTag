// DO NOT INCLUDE - GOING TO DELETE IT IN THE FUTUREs

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "TGUI.hpp"

/**
 * @brief Shape class representing a drawable polygon or circle element.
 *
 * This class allows the creation and rendering of polygons with options for edges,
 * filled faces, and points at vertices. It supports stretching and multiple fill types.
 */
class Shape : public Element
{
public:
    // Member variables
    HyperList<vec2> vertices; ///< List of vertices for the shape.
    HyperList<vec2> fillVertices; ///< List of vertices for the filled shape.
    bool edges;               ///< Whether to render edges.
    bool fill;                ///< Whether to fill the shape.
    bool stretch;             ///< Whether to stretch vertices to the element's dimensions.
    bool points;              ///< Whether to render points at each vertex.

    /**
     * @brief Constructs a Shape object.
     *
     * @param element The base element properties.
     * @param vertices Array of vertices defining the shape.
     * @param vertexCount Number of vertices in the array.
     * 
     * @param stretch Whether to stretch vertices to fit the element's dimensions.
     * @param fill Whether to fill the shape.
     * @param edges Whether to render edges of the shape.
     * @param points Whether to render points at the vertices.
     * @param fillType The fill type for the shape (Triangulate or Fan).
     */
    Shape(const Element &element, vec2 vertices[], int vertexCount, vec2 bool stretch = true, bool fill = false,
          bool edges = true, bool points = false, FillType fillType = Fan)
        : Element(element), stretch(stretch), fill(fill), edges(edges), points(points), fillType(fillType)
    {
        setVertices(vertexCount, vertices);
    }

    /**
     * @brief Renders the shape based on its properties.
     *
     * This method draws the shape by rendering its filled faces, edges, and points.
     */
    virtual void render() override
    {
        if (visible)
        {
            // Render filled faces
            if (fill && vertices.size() > 2)
            {
                switch (fillType)
                {
                case Fan:
                    renderFan();
                    break;
                case Triangulate:
                    renderTriangulate();
                    break;
                }
            }

            // Render edges
            if (edges && vertices.size() > 1)
            {
                for (int i = 0; i < vertices.size(); i++)
                {
                    ivec2 v1 = vertexToPixel(vertices.get(i));
                    ivec2 v2 = vertexToPixel(vertices.get((i + 1) % vertices.size()));
                    // drawLine(v1, v2); // Replace with actual rendering function.
                }
            }

            // Render points
            if (points && vertices.size() > 0)
            {
                for (int i = 0; i < vertices.size(); i++)
                {
                    ivec2 p = vertexToPixel(vertices.get(i));
                    // fillRect(p.x, p.y, 2, 2); // Replace with actual rendering function.
                }
            }

            // Render circle (if vertices.size() == 0)
            if (vertices.size() == 0)
            {
                if (fill)
                {
                    // fillCircle(); // Replace with actual rendering function.
                }
                if (edges)
                {
                    // drawCircle(); // Replace with actual rendering function.
                }
            }
        }
    }

    /**
     * @brief Sets the vertices of the shape.
     *
     * @param vertexCount Number of vertices to set.
     * @param vertices Optional array of vertices to initialize the shape.
     */
    void setVertices(int vertexCount, vec2 vertices[] = nullptr)
    {
        this->vertices.addFromArray(vertices, vertexCount);
    }

    /**
     * @brief Creates vertices for a regular polygon.
     *
     * @param vertexCount Number of vertices to create.
     * @param rotate Initial rotation of the polygon.
     * @param degreesToRadians Whether to interpret the rotation in degrees.
     */
    void createVertices(int vertexCount, float rotate, bool degreesToRadians = false)
    {
        if (vertexCount == 0)
        {
            return;
        }

        float angle = 2.0f * PI / vertexCount;
        vertices.clear();

        for (int i = 0; i < vertexCount; i++)
        {
            vertices.addend(vec2(1, 0).rotate(angle * i));
        }

        rotateVertices(rotate, degreesToRadians);
    }

    /**
     * @brief Rotates the vertices of the shape.
     *
     * @param rotate The angle to rotate the vertices.
     * @param degreesToRadians Whether to interpret the angle in degrees.
     */
    void rotateVertices(float rotate, bool degreesToRadians = false)
    {
        if (degreesToRadians)
        {
            rotate = degToRad(rotate);
        }

        for (int i = 0; i < vertices.size(); i++)
        {
            vertices.get(i).rotate(wrapAngleRad(rotate));
        }
    }

    /**
     * @brief Converts a vertex in local coordinates to screen pixels.
     *
     * @param vertex The vertex in local coordinates.
     * @return ivec2 The vertex position in screen pixels.
     */
    ivec2 vertexToPixel(vec2 vertex)
    {
        ivec2 position = getPosition();
        return ivec2(
            mapFloat(vertex.x, -1.0f, 1.0f, position.x, position.x + scale.x),
            mapFloat(vertex.y, -1.0f, 1.0f, position.y, position.y + scale.y));
    }

private:
    /**
     * @brief Renders the shape using a triangle fan method.
     */
    void renderFan()
    {
        ivec2 v0 = vertexToPixel(vertices.get(0));
        for (int i = 1; i < vertices.size() - 1; i++)
        {
            ivec2 v1 = vertexToPixel(vertices.get(i));
            ivec2 v2 = vertexToPixel(vertices.get(i + 1));
            // fillTriangle(v0, v1, v2); // Replace with actual rendering function.
        }
    }

    /**
     * @brief Renders the shape using triangulation.
     */
    void renderTriangulate()
    {
        HyperList<int> indices;
        for (int i = 0; i < vertices.size(); i++)
        {
            indices.addend(i);
        }

        while (indices.size() > 2)
        {
            for (int i = 0; i < indices.size(); i++)
            {
                int prev = indices.get((i - 1 + indices.size()) % indices.size());
                int curr = indices.get(i);
                int next = indices.get((i + 1) % indices.size());

                vec2 v0 = vertices.get(prev);
                vec2 v1 = vertices.get(curr);
                vec2 v2 = vertices.get(next);

                if (isEar(v0, v1, v2, indices))
                {
                    ivec2 p0 = vertexToPixel(v0);
                    ivec2 p1 = vertexToPixel(v1);
                    ivec2 p2 = vertexToPixel(v2);
                    // fillTriangle(p0, p1, p2); // Replace with actual rendering function.

                    indices.removeAt(i);
                    break;
                }
            }
        }
    }

    /**
     * @brief Determines if a triangle is an "ear" for triangulation.
     *
     * @param v0 The first vertex of the triangle.
     * @param v1 The second vertex of the triangle.
     * @param v2 The third vertex of the triangle.
     * @param indices List of indices representing the polygon.
     * @return true If the triangle is a valid ear.
     * @return false Otherwise.
     */
    bool isEar(vec2 v0, vec2 v1, vec2 v2, const HyperList<int> &indices)
    {
        if (crossProduct(v1 - v0, v2 - v1) <= 0) // Check if the triangle is clockwise
        {
            return false;
        }

        for (int j = 0; j < indices.size(); j++)
        {
            vec2 pt = vertices.get(indices.get(j));
            if (pt != v0 && pt != v1 && pt != v2 && pointInTriangle(pt, v0, v1, v2))
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Checks if a point lies inside a triangle.
     *
     * @param pt The point to check.
     * @param v0 The first vertex of the triangle.
     * @param v1 The second vertex of the triangle.
     * @param v2 The third vertex of the triangle.
     * @return true If the point is inside the triangle.
     * @return false Otherwise.
     */
    bool pointInTriangle(vec2 pt, vec2 v0, vec2 v1, vec2 v2)
    {
        float d1 = crossProduct(pt - v0, v1 - v0);
        float d2 = crossProduct(pt - v1, v2 - v1);
        float d3 = crossProduct(pt - v2, v0 - v2);

        bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(hasNeg && hasPos);
    }

    /**
     * @brief Computes the cross product of two 2D vectors.
     *
     * The cross product of two vectors in 2D space returns a scalar value,
     * which is the magnitude of the vector perpendicular to the plane defined
     * by the two input vectors.
     *
     * @param v1 The first 2D vector.
     * @param v2 The second 2D vector.
     * @return float The cross product of the two vectors.
     */
    float crossProduct(const vec2 &v1, const vec2 &v2)
    {
        return v1.x * v2.y - v1.y * v2.x;
    }
};

#endif // SHAPE_HPP