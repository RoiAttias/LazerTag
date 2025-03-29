#ifndef DEVICEBOX_HPP
#define DEVICEBOX_HPP

#include "MANAGER/GUI_Manager/GUI_Manager.hpp"

/**
 * @brief DeviceBox element composed of a background rectangle and text,
 * used to display device information in the format "id | deviceGroup".
 */
class DeviceBox : public Textbox {
public:
    int deviceId;       ///< The device ID.
    int deviceGroup;   ///< The device type.
    bool selected = false; ///< Whether the DeviceBox is selected.

    uint32_t color1T = TFT_VIOLET;
    uint32_t color1F = TFT_SKYBLUE;
    uint32_t color2T = TFT_BROWN;
    uint32_t color2F = TFT_YELLOW;
    int corner1 = 20;
    int corner2 = 5;

    uint32_t textColor = color1T;
    uint32_t fillColor = color1F;
    int cornerRadius = corner1;

    /**
     * @brief Construct a DeviceBox element.
     * 
     * @param element Base element properties.
     * @param deviceId The device ID.
     * @param deviceGroup The device type.
     */
    DeviceBox(const Element &element, int deviceId = 0, int deviceGroup = 0)
        : Textbox(element, "", textColor, fillColor, textColor, 1, MC_DATUM,
            1, cornerRadius, &FreeMono18pt7b, true, true),
        deviceId(deviceId), deviceGroup(deviceGroup)
        {
            // Update the text content.
            updateInformation(deviceId, deviceGroup);
            setSelected(selected);
        }

    /**
     * @brief Update the device information displayed by the DeviceBox.
     *
     * @param id The device ID.
     * @param group The device group.
     */
    void updateInformation(int id, int group) {
        deviceId = id;
        deviceGroup = group;
        // Format the text content as "(id | deviceGroup)".
        Textbox::text.content = String(deviceId) + "|" + deviceGroupString(deviceGroup & 0xFF);
        // Mark the element for re-rendering.
        Element::callRender();
    }

    /**
     * @brief Set the selected state of the DeviceBox.
     *
     * @param selected Whether the DeviceBox is selected.
     */
    void setSelected(bool selected) {
        this->selected = selected;
        // Update the text color based on the selected state.
        textColor = selected ? color2T : color1T;
        fillColor = selected ? color2F : color1F;
        cornerRadius = selected ? corner2 : corner1;

        Textbox::background.fillColor = fillColor;
        Textbox::background.borderColor = textColor;
        Textbox::text.textColor = textColor;
        Textbox::background.cornerRadius = cornerRadius;
        // Mark the element for re-rendering.
        Element::callRender();
    }

    /**
     * @brief Invert the selected state of the DeviceBox.
     */
    void invertSelected() {
        setSelected(!selected);
    }
};

#endif // DEVICEBOX_HPP