#ifndef ACTIVITY_HPP
#define ACTIVITY_HPP

#include "Element.hpp"
#include "LuminaUI.hpp"

/**
 * @brief Activity class serves as a container for managing multiple child elements.
 *
 * Activity provides a layout mechanism for managing child elements, handling rendering,
 * updating positions, and forwarding events like Touchs to child elements.
 *
 * Key Features:
 * - **Position and Scale**: Manages the overall position and size of the activity area.
 * - **Child Elements**: Allows adding, removing, and updating multiple child elements.
 * - **Rendering**: Renders child elements when the activity is visible.
 * - **Event Handling**: Propagates Touch events to relevant child elements within bounds.
 */
class Activity : public Element
{
public:
    HyperList<Element *> elements; // List of child elements managed by the Activity.

    // Constructors
    /**
     * @brief Constructor of Activity
     *
     * @param origin The origin position of the activity on the X and Y axis - for full screen: should be zero.
     * @param scale The width and height of the activity - for full screen: should be screen's diamensions.
     * @param visible Enable rendering for the activity.
     * @param renderAlways Flag to render the activity without calls.
     * @param OnTouch_enable Enable or disable the Touch event for the activity.
     * @param OnTouch_handler Pointer to the function to handle the Touch event.
     */
    Activity(ivec2 origin = LuminaUI_AUTO, ivec2 scale = LuminaUI_AUTO, bool visible = true, bool renderAlways = true,
        bool OnTouch_enable = false, TouchEvent OnTouch_handler = nullptr)
        : Element(origin, ivec2(), scale, visible, renderAlways, OnTouch_enable, OnTouch_handler){
            if (origin == LuminaUI_AUTO) {
                origin = ivec2(0, 0);
            }
            if (scale == LuminaUI_AUTO) {
                scale = this->scale;
            }
        }

    Activity(const Element &element) {
        Activity(element.origin, element.scale, element.visible, element.renderAlways, 
            element.OnTouch_enable, element.OnTouch_handler);
        }

    // Overrides
    /**
     * @brief Render the Activity and all its child elements.
     * @param viewport The viewport to render the activity in.
     * @return The clamped viewport of the activity.
     */
    virtual Viewport render(const Viewport &viewport) override {
        // Call the base class's render function
        Viewport activityViewport = Element::render(viewport); // Get the clamped viewport of the activity
        updateViewports();
        if (visible) {
            // Render all child elements in the activity
            Element* element; // Pointer to an element
            Viewport elementViewport; // Viewport of an element
            // Render all visible child elements - first in back
            for (int i = 0; i < getElementCount(); i++) {
                element = getElement(i);
                if (element != nullptr) {
                    elementViewport = element->scale != LuminaUI_AUTO ? element->getViewport() : activityViewport;
                    if (element->visible && element->shouldRender() && activityViewport.inRange(elementViewport)) { 
                        elementViewport = elementViewport.clamp(activityViewport);
                        element->render(elementViewport);
                    }
                }
            }
        }
        return activityViewport;
    }

    /**
     * @brief Handles Touch events and forwards them to child elements if necessary.
     *
     * @param point The location where the Touch occurred.
     */
    virtual void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override
    {
        // Call the base class's Touch event handler
        Element::OnTouch_execute(point,touchStatus);
        updateViewports();
        Element *element; bool continueLoop = true;
        // Executes Touch events in reversed order, opposite of rendering order
        // the last elements inside the list are seen in front.
        for (int i = getElementCount() - 1; i >= 0 && continueLoop; i--)
        {
            element = getElement(i);
            // Check if the element is not null
            if (element != nullptr){
                /**
                 * Conditions for executing the Touch event:
                 * - The element is visible.
                 * - The element is in range of the activity.
                 * - The element has Touch event enabled.
                 * - The element has a Touch event handler.
                 * 
                 * Case 1: The Touch event occurred within the element's bounds:
                 * - Execute the Touch event handler.
                 * - Stop the loop.
                 * 
                 * Case 2: The Touch event is a RELEASE event:
                 * - Execute the READY Touch event handler for all elements
                 *   that DO NOT get the RELEASE Touch event at the end of the touch event.
                 * - Continue the loop to execute the READY Touch event for all elements.
                 */
                if (element->visible && inRange(element) && element->OnTouch_enable && element->OnTouch_handler != nullptr)
                {
                    // Check if the Touch event occurred within the element's bounds
                    if (element->inRange(point)){
                        // Execute the Touch event handler
                        element->OnTouch_execute(point, touchStatus);
                        // Mark that the Touch event was handled and stop the loop
                        // in order to prevent other elements from receiving the Touch event.
                        continueLoop = false;
                    }
                    // Check if the Touch event is a RELEASE event
                    if (touchStatus == TouchStatus_RELEASE){
                        // Execute the READY Touch event handler for all elements
                        // that DO NOT get the RELEASE Touch event at the end of the touch event.
                        if (continueLoop == true){
                            ivec2 point = element->getPosition();
                            element->OnTouch_execute(point, TouchStatus_READY);
                        }
                        // Continue the loop to execute the READY Touch event for all elements.
                        continueLoop = true;
                    }
                }
            }
        }
    }

    // Calculations
    /**
     * @brief Updates the positions and scales of all child elements relative to the Activity's location.
     */
    virtual void updateViewports()
    {
        Element *element;
        for (size_t i = 0; i < getElementCount(); i++)
        {
            // Get the element from the list
            element = getElement(i);

            if (element == nullptr)
            {
                continue;
            }

            // Default origin is (0,0) if not set
            if (element->origin == LuminaUI_AUTO)
            {
                element->origin = this->origin;
            }

            // Position each element relative to the Activity's position
            element->offset = this->getViewport().position;

            // Default scale is the Activity's scale if not set
            if (element->scale == LuminaUI_AUTO)
            {
                element->scale = this->scale;
            }
        }
    }

    // Element Management
    /**
     * @brief Add a child element to the Activity.
     *
     * @param element Pointer to the element to be added.
     */
    void addElement(Element *element)
    {
        if (element != nullptr)
        {
            elements.addend(element);
        }
    }

    /**
     * @brief Add child elements to the Activity from array.
     *
     * @param element Pointer to the first element in the array to be added.
     * @param size Size of the array.
     */
    void addElements(Element ** array, int size)
    {
        if (array != nullptr)
        {
            elements.addFromArray(array,size);
        }
    }

    /**
     * @brief Remove a child element from the Activity.
     *
     * @param index Index of the element to be removed.
     */
    void removeElement(int index)
    {
        elements.remove(index);
    }

    /**
     * @brief Remove a child element from the Activity.
     *
     * @param element Pointer to the element to be removed.
     */
    void removeElement(Element *element)
    {
        for (int i = 0; i < elements.size(); i++)
        {
            if (getElement(i) == element)
            {
                removeElement(i);
                break;
            }
        }
    }

    /**
     * @brief Remove all child elements from the Activity.
     */
    void clearElements()
    {
        elements.clear();
    }

    /**
     * @brief Get the number of child elements in the Activity.
     *
     * @return The number of child elements.
     */
    int getElementCount() const
    {
        return elements.size();
    }

    /**
     * @brief Access a child element by index.
     *
     * @param index The index of the element.
     * @return Pointer to the element, or nullptr if out of bounds.
     */
    Element *getElement(int index)
    {
        return (index >= 0 && index < getElementCount()) ? elements.get(index) : nullptr;
    }
};
#endif // ACTIVITY_HPP