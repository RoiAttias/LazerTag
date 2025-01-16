#ifndef TABLE_HPP
#define TABLE_HPP

#include "TFT_eSPI_Elements.hpp"
#include <TFT_eSPI.h>

/**
 * @brief A table element that renders the grid lines of a Grid object.
 */
class Table : public Element {
public:
    Grid* grid; ///< Pointer to the Grid object

    /**
     * @brief Constructor for Table.
     * @param grid Pointer to the Grid object.
     */
    Table(Grid* grid = nullptr) : grid(grid) {}

    /**
     * @brief Render only the grid lines (rows and columns) for the table.
     * @param viewport The viewport to render the table in.
     * @return The clamped viewport of the table.
     */
    virtual Viewport render(const Viewport& viewport) override {
        Viewport tableViewport = Element::render(viewport);
        
        if (grid != nullptr) {
            // Calculate and render the vertical lines (columns)
            int xPos = tableViewport.position.x;
            for (int i = 0; i < grid->columnDefinitions.size(); i++) {
                ColumnDefinition colDef = grid->columnDefinitions.get(i);
                TGUI::tft_instance->drawFastVLine(xPos, tableViewport.position.y, tableViewport.scale.y, TFT_WHITE);
                xPos += colDef.width;
            }
            TGUI::tft_instance->drawFastVLine(xPos, tableViewport.position.y, tableViewport.scale.y, TFT_WHITE);

            // Calculate and render the horizontal lines (rows)
            int yPos = tableViewport.position.y;
            for (int i = 0; i < grid->rowDefinitions.size(); i++) {
                RowDefinition rowDef = grid->rowDefinitions.get(i);
                TGUI::tft_instance->drawFastHLine(tableViewport.position.x, yPos, tableViewport.scale.x, TFT_WHITE);
                yPos += rowDef.height;
            }
            TGUI::tft_instance->drawFastHLine(tableViewport.position.x, yPos, tableViewport.scale.x, TFT_WHITE);
        }
        
        return tableViewport;
    }
};

#endif // TABLE_HPP