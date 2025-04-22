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
    uint32_t color; ///< Color of the grid lines

    /**
     * @brief Constructor for Table.
     * @param grid Pointer to the Grid object.
     */
    Table(Grid* grid = nullptr, uint32_t color = TFT_WHITE) : grid(grid), color(color) {}

    /**
     * @brief Render only the grid lines (rows and columns) for the table.
     * @param viewport The viewport to render the table in.
     * @return The clamped viewport of the table.
     */
    virtual Viewport render(const Viewport& viewport) override {
        Viewport tableViewport = {ivec2(0, 0), ivec2(0, 0)};
        
        if (grid != nullptr) {
            // Set the viewport to the grid's position and size
            origin = grid->origin;
            offset = grid->offset;
            scale = grid->scale;
            tableViewport = Element::render(viewport);
            
            // Set the viewport for the TFT display
            LuminaUI::tft_instance->setViewport(offset.x, offset.y,
                tableViewport.scale.x, tableViewport.scale.y);
            
            // Get the x and y positions for rendering
            int xPos = getPosition().x - 1;
            int yPos = getPosition().y - 1;

            // Calculate and render the vertical lines (columns)
            LuminaUI::tft_instance->fillRect(xPos, yPos, 2, scale.y, color);
            for (int i = 0; i < grid->columnDefinitions.size(); i++) {
                ColumnDefinition colDef = grid->columnDefinitions.get(i);
                xPos += colDef.width;
                LuminaUI::tft_instance->fillRect(xPos, yPos, 2, scale.y, color);
            }

            // Reset the xPos and yPos for rendering rows
            xPos = getPosition().x - 1;
            yPos = getPosition().y - 1;

            // Calculate and render the horizontal lines (rows)
            LuminaUI::tft_instance->fillRect(xPos, yPos, scale.x, 2, color);
            for (int i = 0; i < grid->rowDefinitions.size(); i++) {
                RowDefinition rowDef = grid->rowDefinitions.get(i);
                yPos += rowDef.height;
                LuminaUI::tft_instance->fillRect(xPos, yPos, scale.x, 2, color);
            }

            // Reset the viewport to the original state
            LuminaUI::tft_instance->resetViewport();
        }

        // Reset the origin, offset, and scale to default values to avoid touch issues
        origin = ivec2(0, 0);
        offset = ivec2(0, 0);
        scale = ivec2(0, 0);
        
        return tableViewport;
    }
};

#endif // TABLE_HPP