#ifndef ACTIVITY_HPP
#define ACTIVITY_HPP

#include "Element.hpp"

/**
 * @brief Activity class serves as a container for managing multiple child elements.
 *
 * Activity provides a layout mechanism for managing child elements, handling rendering,
 * updating positions, and forwarding events like clicks to child elements.
 *
 * Key Features:
 * - **Position and Scale**: Manages the overall position and size of the activity area.
 * - **Child Elements**: Allows adding, removing, and updating multiple child elements.
 * - **Rendering**: Renders child elements when the activity is visible.
 * - **Event Handling**: Propagates click events to relevant child elements within bounds.
 */
class Activity : public Element
{
public:
    HyperList<Element *> elements; // List of child elements managed by the Activity.

    // Constructors
    /**
     * @brief Constructor of Activity
     *
     * @param position The position of the activity on the X and Y axis.
     * @param scale The width and height of the activity.
     * @param visible Enable rendering for the activity.
     * @param OnClick_enable Enable or disable the click event for the activity.
     * @param OnClick_handler Pointer to the function to handle the click event.
     */
    Activity(ivec2 position = ivec2(), ivec2 scale = ivec2(), bool visible = true, bool OnClick_enable = false, std::function<void(ivec2)> OnClick_handler = nullptr)
        : Element(position, vec2(), scale, visible, OnClick_enable, OnClick_handler) {}

    // Overrides
    /**
     * @brief Render the Activity and all its child elements.
     */
    virtual void render() override
    {
        if (visible)
        {
            // Update the positions of all child elements relative to the Activity's location.
            updatePositions();
            // Render all visible child elements
            for (int i = 0; i < elements.size(); i++)
            {
                if (elements.get(i)->visible && inRange(elements.get(i)))
                {
                    elements.get(i)->render();
                }
            }
        }
    }

    /**
     * @brief Handles click events and forwards them to child elements if necessary.
     *
     * @param point The location where the click occurred.
     */
    virtual void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override
    {
        if (OnTouch_enable && OnTouch_handler && TouchStatus::ready) {
            OnTouch_handler(point,touchStatus);
        }
    

        // Executes click events in reversed order, opposite of rendering order
        // the last elements inside the list are in front.
        for (int i = elements.size() - 1; i >= 0 ; i--)
        {
            // Execute click events if child is visible and inside activity range
            if (elements.get(i)->isVisible() && inRange(elements.get(i)))
            {
                // Execute click events to child elements if the point is in their range
                if (elements.get(i)->inRange(point))
                {
                    elements.get(i)->OnClick_execute(point);
                }
            }
        }
    }

    // Calculations
    /**
     * @brief Updates the positions of all child elements relative to the Activity's location.
     */
    virtual void updatePositions()
    {
        for (int i = 0; i < elements.size(); i++)
        {
            // Position each element relative to the Activity's position
            elements.get(i)->setOffset(finalPosition);
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
            elements.add(element);
        }
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
            if (elements.get(i) == element)
            {
                elements.remove(i);
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
        return (index >= 0 && index < elements.size()) ? elements.get(index) : nullptr;
    }
};

#endif // ACTIVITY_HPP