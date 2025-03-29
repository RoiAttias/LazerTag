#ifndef LOADARC_HPP
#define LOADARC_HPP

#include "MANAGER/GUI_Manager/GUI_Manager.hpp"

/**
 * @brief LoadArc class represents a loading arc element with a background rectangle.
 *
 * The LoadArc class is used to create a loading arc with customizable properties such as
 * colors, angles, thickness, and corner radius. It inherits from the Element class and
 * provides methods for rendering the arc and setting its angles and percentage.
 */
class LoadArc : public Element {
public:
    Rectangle background; ///< Background rectangle of the arc.
    Arc arc;             ///< Arc element for rendering.

    uint32_t foregroundColor; ///< Color of the arc.
    uint32_t backgroundColor; ///< Background color of the arc.
    int startAngle;   ///< Start angle in degrees [0..360].
    int sweepAngle;   ///< Sweep (arc) angle in degrees.
    int thickness;    ///< Desired arc border thickness in pixels (arc thickness = r - ir + 1).
    bool roundEnds;   ///< Whether to round the arc's ends.
    int cornerRadius; ///< Corner radius for the background rectangle.

    /**
     * @brief Constructor for LoadArc.
     *
     * @param element Base element properties.
     * @param foregroundColor Color of the arc.
     * @param backgroundColor Background color of the arc.
     * @param startAngle Start angle in degrees [0..360].
     * @param sweepAngle Sweep (arc) angle in degrees.
     * @param thickness Desired arc border thickness in pixels (arc thickness = r - ir + 1).
     * @param roundEnds Whether to round the arc's ends.
     * @param cornerRadius Corner radius for the background rectangle.
     */
    LoadArc(const Element &element, uint32_t foregroundColor = TFT_WHITE,
        uint32_t backgroundColor = TFT_BLACK, int startAngle = 0,
        int sweepAngle = 90, int thickness = 5, bool roundEnds = false, int cornerRadius = 30) :
        Element(element), foregroundColor(foregroundColor),
        backgroundColor(backgroundColor), startAngle(startAngle),
        sweepAngle(sweepAngle), thickness(thickness), roundEnds(roundEnds),
        cornerRadius(cornerRadius),
        background(Rectangle(element, backgroundColor, foregroundColor, true, true, cornerRadius)),
        arc(Arc(element, foregroundColor, backgroundColor, startAngle, sweepAngle, thickness, roundEnds)) {}
    
    /**
     * @brief Render the LoadArc element.
     *
     * @param viewport The viewport to render in.
     * @return The clamped viewport.
     */
    Viewport render(const Viewport &viewport) override {
        arc.scale = scale / 2; // Set the arc's scale to half of the element's scale.
        arc.offset = getPosition() +(scale / 2); // Set the arc's origin to the center of the element.
        background.scale = scale; // Set the background's scale to the element's scale.
        background.offset = getPosition(); // Set the background's origin to the element's origin.

        // Render the parent element and set the viewport for this element.
        Viewport arcViewport = Element::render(viewport);
        LuminaUI::tft_instance->setViewport(arcViewport.position.x, arcViewport.position.y, arcViewport.scale.x, arcViewport.scale.y);

        // Set the background rectangle's viewport and render it.
        background.render(arcViewport);

        // Set the arc's viewport and render it.
        arc.render(arcViewport);

        // Reset the viewport back to the full display.
        LuminaUI::tft_instance->resetViewport();
        return arcViewport;
    }

    /**
     * @brief Set the angles for the arc.
     * 
     * @param startAngle The start angle in degrees [0..360].
     * @param sweepAngle The sweep (arc) angle in degrees.
     */
    void setAngles(int startAngle, int sweepAngle) {
        this->startAngle = startAngle;
        this->sweepAngle = sweepAngle;
        arc.startAngle = startAngle;
        arc.sweepAngle = sweepAngle;
        callRender();
    }

    /**
     * @brief Set the percentage of the arc.
     *
     * @param value The percentage value [0.0..1.0].
     */
    void setPercent(float value) {
        setAngles(0, int(value * 360));
    }
};

#endif // LOADARC_HPP