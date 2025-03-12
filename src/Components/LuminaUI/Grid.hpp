#ifndef GRID_HPP
#define GRID_HPP

#include "LuminaUI.hpp"

/**
 * @brief Defines the column's properties, such as width.
 */
struct ColumnDefinition {
    int width; ///< Width of the column

    ColumnDefinition(int width = 0) : width(width) {}
};

/**
 * @brief Defines the row's properties, such as height.
 */
struct RowDefinition {
    int height; ///< Height of the row

    RowDefinition(int height = 0) : height(height) {}
};

/**
 * @brief A cell in the grid that contains an element and its location in the grid.
 */
struct Cell {
    Element* element;  ///< Pointer to the element contained in this cell
    ivec2 location;    ///< Location of the cell in the grid (column, row) using zero-based indexing

    Cell(Element* element = nullptr, ivec2 location = ivec2()) : element(element), location(location) {}
};

/**
 * @brief A grid layout that manages columns, rows, and cells with child elements.
 * 
 * This class updates cell positions dynamically based on the cumulative sizes
 * of the columns and rows, and gracefully handles cells that are out of range.
 */
class Grid : public Element {
public:
    HyperList<ColumnDefinition> columnDefinitions; ///< List of column definitions
    HyperList<RowDefinition> rowDefinitions;         ///< List of row definitions
    HyperList<Cell> cells;                           ///< List of cells with child elements

    /**
     * @brief Construct a grid.
     * @param element The element to inherit parameters from.
     */
    Grid(const Element& element) : Element(element) {}

    /**
     * @brief Add a column to the grid.
     * @param width The width of the column.
     */
    void addColumn(int width) {
        columnDefinitions.addend(ColumnDefinition(width));
    }

    /**
     * @brief Add a row to the grid.
     * @param height The height of the row.
     */
    void addRow(int height) {
        rowDefinitions.addend(RowDefinition(height));
    }

    /**
     * @brief Add a cell to the grid.
     * @param cell The Cell object to add.
     * @return True if the cell was added, false if the location is already occupied or the element is null.
     */
    bool addCell(Cell cell) {
        // Check for null elements.
        if (cell.element == nullptr)
            return false;
        
        // Check for duplicate locations.
        for (int i = 0, n = cells.size(); i < n; i++) {
            if (cells.get(i).location == cell.location)
                return false;
        }

        // Add the cell to the list.
        cells.addend(cell);
        return true;
    }

    /**
     * @brief Add a cell by specifying an element and its grid location.
     * @param element Pointer to the element.
     * @param location The grid location (column, row).
     * @return True if the cell was added successfully.
     */
    bool addCell(Element* element, ivec2 location) {
        return addCell(Cell(element, location));
    }

    /**
     * @brief Get the grid dimensions as (number of columns, number of rows).
     */
    ivec2 getCells() {
        return ivec2(columnDefinitions.size(), rowDefinitions.size());
    }

    /**
     * @brief Update positions and sizes for all in-range cells.
     * 
     * For each cell whose location is within the defined ranges,
     * the element's offset and scale are set based on the cumulative sizes.
     *
     * @param viewport The viewport of the grid.
     */
    void updateViewports(const Viewport &viewport) {
        // Get the number of columns and rows.
        const int numCols = columnDefinitions.size();
        const int numRows = rowDefinitions.size();

        // Precompute cumulative offsets for columns and rows.
        int* colOffsets = new int[numCols];
        int* rowOffsets = new int[numRows];

        int offset = 0;
        for (int i = 0; i < numCols; i++) {
            colOffsets[i] = offset;
            offset += columnDefinitions.get(i).width;
        }

        offset = 0;
        for (int i = 0; i < numRows; i++) {
            rowOffsets[i] = offset;
            offset += rowDefinitions.get(i).height;
        }

        // Update each cell's element position and size.
        for (int i = 0, n = cells.size(); i < n; i++) {
            // Get fast access to the cell.
            Cell* cell = cells.getPointer(i);

            // Skip if the element is null or the location is out of range.
            if (cell->element == nullptr ||
                cell->location.x < 0 || cell->location.x >= numCols ||
                cell->location.y < 0 || cell->location.y >= numRows) {
                continue;
            }

            ivec2 point = viewport.position;
            point.x += colOffsets[cell->location.x];
            point.y += rowOffsets[cell->location.y];

            ivec2 newScale;
            newScale.x = columnDefinitions.get(cell->location.x).width;
            newScale.y = rowDefinitions.get(cell->location.y).height;
            
            // Set the element's offset and scale.
            cell->element->offset = point;
            cell->element->scale = newScale;
        }

        // Clean up.
        delete[] colOffsets;
        delete[] rowOffsets;
    }

    /**
     * @brief Render the grid and its child elements.
     * 
     * The grid updates cell positions and renders each in-range child element.
     *
     * @param viewport The viewport to render in.
     * @return The clamped viewport of the grid.
     */
    virtual Viewport render(const Viewport &viewport) override {
        Viewport gridViewport = Element::render(viewport);
        updateViewports(gridViewport);
        if (visible) {
            for (int i = 0, n = cells.size(); i < n; i++) {
                Cell* cell = cells.getPointer(i);
                if (cell->element != nullptr) {
                    Element* cellElement = cell->element;
                    if (cellElement->visible && cellElement->shouldRender() &&
                        gridViewport.inRange(cellElement->getViewport())) {
                        Viewport cellViewport = (cellElement->scale != LuminaUI_AUTO)
                                                  ? cellElement->getViewport()
                                                  : gridViewport;
                        cellViewport = cellViewport.clamp(gridViewport);
                        cellElement->render(cellViewport);
                    }
                }
            }
        }
        return gridViewport;
    }

    /**
     * @brief Handle touch events and forward them to child elements.
     * 
     * Updates positions and then forwards the touch event to any in-range child element.
     *
     * @param point The point where the touch occurred.
     * @param touchStatus The status of the touch event.
     */
    virtual void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override {
        Element::OnTouch_execute(point, touchStatus);
        updateViewports(getViewport());
        for (int i = 0, n = cells.size(); i < n; i++) {
            Cell* cell = cells.getPointer(i);
            if (cell->element != nullptr && cell->element->inRange(point)) {
                cell->element->OnTouch_execute(point, touchStatus);
            }
        }
    }
};

#endif // GRID_HPP