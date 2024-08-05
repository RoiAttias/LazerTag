#ifndef GRID_HPP
#define GRID_HPP

#include "TGUI.hpp"

struct ColumnDefinition
{
    int width; // Use -1 for automatic width
    int minAuto;
    int renderWidth; // The width to be used for rendering

    ColumnDefinition(int width, int minAuto = 0) : width(width), minAuto(minAuto), renderWidth(width) {}
};

struct RowDefinition
{
    int height; // Use -1 for automatic height
    int minAuto;
    int renderHeight; // The height to be used for rendering

    RowDefinition(int height, int minAuto = 0) : height(height), minAuto(minAuto), renderHeight(height) {}
};

struct Cell
{
    Element *element;
    Point location;

    Cell(Element *element, Point location) : element(element), location(location);
}

class Grid : public Element
{
public:
    HyperList<ColumnDefinition> columnDefinitions;
    HyperList<RowDefinition> rowDefinitions;
    HyperList<Cell> cells;

    bool addCell(Cell cell)
    {
        HyperList<Point> locations;
        for (int i = 0; i < cells.size; i++)
        {
            locations.add(cells.get(i).location);
        }
        if (locations.contains(cell.location))
        {
            return false;
        }

        cells.add(cell);
        return true;
    }

    void Grid::updatePositions()
    {
        int totalWidth = 0;
        int totalHeight = 0;
        int autoWidthCount = 0;
        int autoHeightCount = 0;

        // Calculate total width and height and count automatic columns/rows
        for (int i = 0; i < columnDefinitions.size(); i++)
        {
            if (columnDefinitions.get(i).width == -1)
            {
                autoWidthCount++;
            }
            else
            {
                totalWidth += columnDefinitions.get(i).width;
            }
        }
        for (int i = 0; i < rowDefinitions.size(); i++)
        {
            if (rowDefinitions.get(i).height == -1)
            {
                autoHeightCount++;
            }
            else
            {
                totalHeight += rowDefinitions.get(i).height;
            }
        }

        // Calculate the width and height for automatic columns/rows based on elements' sizes
        int remainingWidth = 0;
        int remainingHeight = 0;
        if (autoWidthCount > 0)
        {
            remainingWidth = (this->width - totalWidth) / autoWidthCount;
        }
        if (autoHeightCount > 0)
        {
            remainingHeight = (this->height - totalHeight) / autoHeightCount;
        }

        // Adjust columns and rows for minimum sizes and set renderWidth/renderHeight
        for (int i = 0; i < columnDefinitions.size(); i++)
        {
            if (columnDefinitions.get(i).width == -1)
            {
                columnDefinitions.get(i).renderWidth = std::max(remainingWidth, columnDefinitions.get(i).minAuto);
            }
            else
            {
                columnDefinitions.get(i).renderWidth = columnDefinitions.get(i).width;
            }
        }
        for (int i = 0; i < rowDefinitions.size(); i++)
        {
            if (rowDefinitions.get(i).height == -1)
            {
                rowDefinitions.get(i).renderHeight = std::max(remainingHeight, rowDefinitions.get(i).minAuto);
            }
            else
            {
                rowDefinitions.get(i).renderHeight = rowDefinitions.get(i).height;
            }
        }

        // If totalWidth or totalHeight exceeds available space, scale down proportionally
        int usedWidth = 0;
        int usedHeight = 0;
        for (int i = 0; i < columnDefinitions.size(); i++)
        {
            usedWidth += columnDefinitions.get(i).renderWidth;
        }
        for (int i = 0; i < rowDefinitions.size(); i++)
        {
            usedHeight += rowDefinitions.get(i).renderHeight;
        }

        if (usedWidth > this->width)
        {
            float scaleFactor = static_cast<float>(this->width) / usedWidth;
            for (int i = 0; i < columnDefinitions.size(); i++)
            {
                columnDefinitions.get(i).renderWidth = static_cast<int>(columnDefinitions.get(i).renderWidth * scaleFactor);
            }
        }

        if (usedHeight > this->height)
        {
            float scaleFactor = static_cast<float>(this->height) / usedHeight;
            for (int i = 0; i < rowDefinitions.size(); i++)
            {
                rowDefinitions.get(i).renderHeight = static_cast<int>(rowDefinitions.get(i).renderHeight * scaleFactor);
            }
        }

        // Update positions of each cell
        for (int i = 0; i < cells.size(); i++)
        {
            Cell &cell = cells.getAddress(i)->value;

            // Calculate new x position
            int x = 0;
            for (int col = 0; col < cell.location.x; col++)
            {
                x += columnDefinitions.get(col).renderWidth;
            }

            // Calculate new y position
            int y = 0;
            for (int row = 0; row < cell.location.y; row++)
            {
                y += rowDefinitions.get(row).renderHeight;
            }

            // Set the new position to the element
            cell.element->setPosition(x, y);
        }
    }
    

    void render()
    {

        for (int i = 0; i < cells.size(); i++)
        {
            cells.getAddress(i)->element->render();
        }
    }

    void OnClick_execute(Point point)
    {
        for (int i = 0; i < elements.size(); i++)
        {
            if (cells.getAddress(i)->element->inRange(point))
                cells.getAddress(i)->element->OnClick_execute(point);
        }
    }

private:/*
    void OnClick_enableEvent(bool enable);
    void OnClick_setHandler(void (*handler)(Point));*/
};

#endif // GRID_HPP