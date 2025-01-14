#ifndef GRID_HPP
#define GRID_HPP

#include "TGUI.hpp"

/**
 * @brief Defines the column's properties, such as width.
 */
struct ColumnDefinition {
    int width; ///< Width of the column

    /**
     * @brief Constructor for ColumnDefinition
     * @param width Initial width of the column (default is 0)
     */
    ColumnDefinition(int width = 0) : width(width) {}
};

/**
 * @brief Defines the row's properties, such as height.
 */
struct RowDefinition {
    int height; ///< Height of the row

    /**
     * @brief Constructor for RowDefinition
     * @param height Initial height of the row (default is 0)
     */
    RowDefinition(int height = 0) : height(height) {}
};

/**
 * @brief A cell in the grid that contains an element and its location in the grid.
 */
struct Cell {
    Element* element;  ///< Pointer to the element contained in this cell
    ivec2 location;    ///< Location of the cell in the grid (column, row)
    int padding[4];    ///< Padding for the cell (left, top, right, bottom)
    /**
     * @brief Constructor for Cell
     * @param element Pointer to an Element object (default is nullptr)
     * @param location Location of the cell in the grid (default is ivec2())
     * @param padding Padding for the cell (default is {0,0,0,0})
     */
    Cell(Element* element = nullptr, ivec2 location = ivec2(), int *padding = nullptr)
        : element(element), location(location) {
        if (padding != nullptr) {
            for (int i = 0; i < 4; i++) {
                this->padding[i] = padding[i];
            }
        } else {
            for (int i = 0; i < 4; i++) {
                this->padding[i] = 0;
            }
        }
    }
};

/**
 * @brief A grid layout that manages columns, rows, and cells with child elements.
 * 
 * The Grid class extends the Element class to provide a grid layout system, where elements 
 * can be added to specific grid cells, and their positions are updated based on 
 * row and column definitions.
 */
class Grid : public Element {
public:
    HyperList<ColumnDefinition> columnDefinitions; ///< List of column definitions
    HyperList<RowDefinition> rowDefinitions;       ///< List of row definitions
    HyperList<Cell> cells;                         ///< List of cells, each containing an element and its location

    /**
     * @brief Add a cell to the grid.
     * 
     * @param cell The Cell object to add.
     * @return True if the cell was added, false if the location is already occupied.
     */
    bool addCell(Cell cell) {
        int len = cells.size();
        HyperList<ivec2> locations;
        for (int i = 0; i < len; i++) {
            locations.addend(cells.get(i).location);
        }
        if (locations.contains(cell.location)) {
            return false;
        }
        cells.addend(cell);
        return true;
    }

    /**
     * @brief Add a cell to the grid by specifying an element and location.
     * 
     * @param element Pointer to the element to be added to the grid.
     * @param location The grid location (column, row) for the element.
     * @param padding Padding for the cell (left, top, right, bottom)
     * @return True if the cell was added, false if the location is already occupied.
     */
    bool addCell(Element* element, ivec2 location, int padding[4]) {
        Cell cell(element, location, padding);
        return addCell(cell);
    }

    ivec2 getCells() {
        return ivec2(columnDefinitions.size(), rowDefinitions.size());
    }

    /**
     * @brief Update the positions of all elements in the grid based on their locations and the column/row sizes.
     */
    void updatePositions(const Viewport &viewport) {
        Cell* cell;
        ivec2 point;
        for (int i = 0; i < cells.size(); i++) {
            cell = cells.getPointer(i);
            
            point = viewport.positionAfterPadding(cell->padding);

            // Calculate new X position based on column widths
            for (int col = 1; col < cell->location.x; col++) {
                point.x += columnDefinitions.get(col-1).width;
            }

            // Calculate new Y position based on row heights
            for (int row = 1; row < cell->location.y; row++) {
                point.y += rowDefinitions.get(row-1).height;
            }

            // Set the new position to the element
            cell->element->offset = point;
        }
    }

    /**
     * @brief Render the grid and all child elements within it.
     * @param viewport The viewport to render the grid in.
     * @return The clamped viewport of the grid.
     */
    virtual Viewport render(const Viewport &viewport) override {
        // Call the base class's render function
        Viewport gridViewport = Element::render(viewport); // Get the clamped viewport of the grid
        updatePositions(gridViewport);
        if (visible) {
            // Render all child elements in cells
            Cell* cell; // Pointer to a cell
            Element* cellElement; // Pointer to an element in a cell
            Viewport cellViewport; // Viewport of a cell
            for (int i = 0; i < cells.size(); i++) {
                cell = cells.getPointer(i);
                cellElement = cell->element;
                if (cellElement->visible && cellElement->shouldRender() && gridViewport.inRange(cellElement->getViewport())) {
                    cellViewport = cellElement->scale != TGUI_AUTO ? cellElement->getViewport() : gridViewport;
                    cellViewport.scale = cellViewport.scaleInsidePadding(cell->padding);
                    cellViewport = cellViewport.clamp(gridViewport);
                    cellElement->render(cellViewport);
                }
            }
        }
        return gridViewport;
    }

    /**
     * @brief Handle touch events and forward them to child elements.
     * 
     * @param point The point where the touch occurred.
     */
    virtual void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override {
        Element *cellElement;
        
        // Execute touch event for the grid itself
        Element::OnTouch_execute(point,touchStatus);

        // Forward touch events to child elements
        for (int i = 0; i < cells.size(); i++) {
            cellElement = cells.getPointer(i)->element;
            if (cellElement->inRange(point)) {
                cellElement->OnTouch_execute(point,touchStatus);
            }
        }
    }
};

#endif // GRID_HPP