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
    ViewBox(const Element& element, Element* child, ivec2 addOffset = ivec2(0, 0))
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

    /**
     * @brief Update the position of the child element.
     */
    virtual void updatePosition() {
        child->offset = origin + offset + addOffset;
    }

    /**
     * @brief Set the offset of the child element.
     * @param newOffset The new offset.
     */
    virtual void setOffset(ivec2 newOffset) {
        offset = newOffset;
        callRender();
    }

    /**
     * @brief Add an offset to the child element.
     * @param add The offset to add.
     */
    virtual void addToOffset(ivec2 add) {
        addOffset += add;
        callRender();
    }
    

};


#endif // VIEWBOX_HPP