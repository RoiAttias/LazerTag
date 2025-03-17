#ifndef VIEWBOX_HPP
#define VIEWBOX_HPP

#include "LuminaUI.hpp"

/**
 * @brief A view box that contains a bigger element and allows scrolling horizontally and vertically.
 */
class ViewBox : public Element {
public:
    Element* child;  ///< The child element contained in the view box
    ivec2 addOffset; ///< Additional offset to apply to the child element

    /**
     * @brief Construct a view box.
     * @param element The element to inherit parameters from.
     * @param child The child element to contain in the view box.
     * @param addOffset The additional offset to apply to the child element.
     */
    ViewBox(const Element& element, Element* child, ivec2 addOffset = ivec2(0.0f, 0.0f))
        : Element(element), child(child), addOffset(addOffset) {}
    
    /**
     * @brief Render the view box and its child element.
     * 
     * The view box updates the child element's position and renders it in the viewport.
     *
     * @param viewport The viewport to render in.
     * @return The clamped viewport of the view box.
     */
    virtual Viewport render(const Viewport &viewport) override {
        // Update position of the child element
        updatePosition();

        // Render the view box
        Viewport vbViewport = Element::render(viewport);

        // Render the child element
        return child->render(vbViewport);
    }

    virtual void On

    /**
     * @brief Update the position of the child element.
     */
    virtual void updatePosition() {
        child->offset = viewport.position + addOffset;
    }
    

};


#endif // VIEWBOX_HPP