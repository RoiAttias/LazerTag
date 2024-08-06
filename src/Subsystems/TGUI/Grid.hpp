#ifndef GRID_HPP
#define GRID_HPP

#include "TGUI.hpp"

struct ColumnDefinition {
    int width;

    ColumnDefinition(int width = 0) : width(width) {}
};

struct RowDefinition {
    int height;

    RowDefinition(int height = 0) : height(height) {}
};

struct Cell {
    Element *element;
    Point location;

    Cell(Element *element = nullptr, Point location = Point(0,0)) : element(element), location(location) {}
};

class Grid : public Element {
public:
    HyperList<ColumnDefinition> columnDefinitions;
    HyperList<RowDefinition> rowDefinitions;
    HyperList<Cell> cells;

    Grid(int xLocation, int yLocation, int width, int height)
        : Element(xLocation, yLocation, width, height) {}

    bool addCell(Cell cell) {
        HyperList<Point> locations;
        for (int i = 0; i < cells.size(); i++) {
            locations.add(cells.get(i).location);
        }
        if (locations.contains(cell.location)) { return false; }

        cells.add(cell);
        return true;
    }

    bool addCell(Element *element, Point location)
    {
        Cell cell(element, location);
        return addCell(cell);
    }

    void updatePositions() {
        // Update positions of each cell
        for (int i = 0; i < cells.size(); i++) {
            Cell *cell = cells.getAddress(i);

            // Calculate new x position
            int x = xLocation;
            for (int col = 0; col < cell->location.x; col++) {
                x += columnDefinitions.get(col).width;
            }

            // Calculate new y position
            int y = yLocation;
            for (int row = 0; row < cell->location.y; row++) {
                y += rowDefinitions.get(row).height;
            }

            // Set the new position to the element
            cell->element->setRenderPositionByPoint(Point(x,y));
        }
    }

    // Override render to include child elements
    virtual void render() override {
        if (visible) {
            // Render all child elements
            for (int i = 0; i < cells.size(); i++) {
                if (cells.getAddress(i)->element->isVisible()) 
                    cells.getAddress(i)->element->render();
            }
        }
    }

    // Override OnClick_execute to handle clicks and forward to child elements
    virtual void OnClick_execute(Point point) override {
        if (OnClick_enable && OnClick_handler != nullptr) {
            OnClick_handler(point);
        }
        
        // Forward click events to child elements
        for (int i = 0; i < cells.size(); i++) {
            if (cells.getAddress(i)->element->inRange(point)) {
                cells.getAddress(i)->element->OnClick_execute(point);
            }
        }
    }
};

#endif // GRID_HPP