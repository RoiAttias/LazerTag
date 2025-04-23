/**
 * @file DeviceBox.hpp
 * @brief Defines the DeviceBox GUI element for the Manager interface.
 *
 * The DeviceBox element displays a device's ID and type (Gun or Vest) in a
 * selectable box. It inherits from Textbox and updates its appearance based
 * on its selected state.
 */

 #ifndef DEVICEBOX_HPP
 #define DEVICEBOX_HPP
 
 #include "MANAGER/GUI_Manager/GUI_Manager.hpp"
 
 /**
  * @class DeviceBox
  * @brief A selectable box showing a device's ID and group.
  *
  * The DeviceBox element is composed of a background rectangle and centered
  * text, formatted as "ID|GroupName". It changes color, border, and corner
  * radius when selected or deselected.
  */
 class DeviceBox : public Textbox {
 public:
     int deviceId;            ///< Numeric ID of the device.
     int deviceGroup;         ///< Group bitmask representing device type.
     bool selected = false;   ///< Selection state of the box.
 
     // Colors and corner radii for two states: true (selected) and false (deselected)
     uint32_t color1T = TFT_VIOLET;    ///< Text color when not selected.
     uint32_t color1F = TFT_SKYBLUE;   ///< Fill color when not selected.
     uint32_t color2T = TFT_BROWN;     ///< Text color when selected.
     uint32_t color2F = TFT_YELLOW;    ///< Fill color when selected.
     int corner1   = 20;               ///< Corner radius when not selected.
     int corner2   = 5;                ///< Corner radius when selected.
 
     uint32_t textColor   = color1T;   ///< Current text color.
     uint32_t fillColor   = color1F;   ///< Current fill color.
     int      cornerRadius = corner1;  ///< Current corner radius.
 
     /**
      * @brief Construct a new DeviceBox.
      * @param element      Base Element properties (position, size, etc.).
      * @param deviceId     The numeric ID to display.
      * @param deviceGroup  The group bitmask (uses Common::deviceGroupString).
      */
     DeviceBox(const Element &element, int deviceId = 0, int deviceGroup = 0)
         : Textbox(element, "",            // Initial empty text
                   textColor,             // Text color
                   fillColor,             // Fill color
                   textColor,             // Border color
                   1, MC_DATUM,            // Text size and datum
                   1, cornerRadius,        // Line spacing and corner radius
                   &FreeMono18pt7b,        // Font
                   true, true),           // Render fill and border
           deviceId(deviceId),
           deviceGroup(deviceGroup)
     {
         updateInformation(deviceId, deviceGroup);
         setSelected(selected);
     }
 
     /**
      * @brief Update the displayed ID and group.
      * @param id     New device ID.
      * @param group  New device group bitmask.
      *
      * Formats the text as "ID|GroupName" and marks the element for re-render.
      */
     void updateInformation(int id, int group) {
         deviceId    = id;
         deviceGroup = group;
         // Format "ID|GroupName"
         Textbox::text.content = String(deviceId)
                               + "|" 
                               + deviceGroupString(deviceGroup & 0xFF);
         Element::callRender();
     }
 
     /**
      * @brief Set the selection state.
      * @param sel  True to select, false to deselect.
      *
      * Adjusts text color, fill color, corner radius, and border to reflect
      * the new state, then marks for re-render.
      */
     void setSelected(bool sel) {
         selected = sel;
         // Choose colors and radius based on state
         textColor    = selected ? color2T : color1T;
         fillColor    = selected ? color2F : color1F;
         cornerRadius = selected ? corner2 : corner1;
 
         // Apply to background and text
         Textbox::background.fillColor   = fillColor;
         Textbox::background.borderColor = textColor;
         Textbox::background.cornerRadius= cornerRadius;
         Textbox::text.textColor         = textColor;
 
         Element::callRender();
     }
 
     /**
      * @brief Toggle the selection state.
      *
      * Inverts the current selection and applies updated appearance.
      */
     void invertSelected() {
         setSelected(!selected);
     }
 };
 
 #endif // DEVICEBOX_HPP 