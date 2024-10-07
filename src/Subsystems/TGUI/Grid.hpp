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

    /**
     * @brief Constructor for Cell
     * @param element Pointer to an Element object (default is nullptr)
     * @param location Location of the cell in the grid (default is ivec2())
     */
    Cell(Element* element = nullptr, ivec2 location = ivec2()) : element(element), location(location) {}
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
            locations.add(cells.get(i).location);
        }
        if (locations.contains(cell.location)) {
            return false;
        }
        cells.add(cell);
        return true;
    }

    /**
     * @brief Add a cell to the grid by specifying an element and location.
     * 
     * @param element Pointer to the element to be added to the grid.
     * @param location The grid location (column, row) for the element.
     * @return True if the cell was added, false if the location is already occupied.
     */
    bool addCell(Element* element, ivec2 location) {
        Cell cell(element, location);
        return addCell(cell);
    }

    /**
     * @brief Update the positions of all elements in the grid based on their locations and the column/row sizes.
     */
    void updatePositions() {
        for (int i = 0; i < cells.size(); i++) {
            Cell* cell = cells.getAddress(i);

            // Calculate new X position based on column widths
            int x = position.x;
            for (int col = 0; col < cell->location.x; col++) {
                x += columnDefinitions.get(col).width;
            }

            // Calculate new Y position based on row heights
            int y = position.y;
            for (int row = 0; row < cell->location.y; row++) {
                y += rowDefinitions.get(row).height;
            }

            // Set the new position to the element
            cell->element->setPosition(ivec2(x, y));
        }
    }

    /**
     * @brief Render the grid and all child elements within it.
     */
    virtual void render() override {
        if (visible) {
            // Render all child elements
            for (int i = 0; i < cells.size(); i++) {
                if (cells.getAddress(i)->element->isVisible()) {
                    cells.getAddress(i)->element->render();
                }
            }
        }
    }

    /**
     * @brief Handle click events and forward them to child elements.
     * 
     * @param point The point where the click occurred.
     */
    virtual void OnClick_execute(ivec2 point) override {
        if (OnClick_enable && OnClick_handler) {
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