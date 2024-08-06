#ifndef ACTIVITY_HPP
#define ACTIVITY_HPP

#include "Element.hpp"

class Activity : public Element {
public:
    // Constructor
    Activity(int xLocation, int yLocation, int width, int height)
        : Element(xLocation, yLocation, width, height) {}

    virtual void updatePositions()  {
        for (int i = 0; i < elements.size(); i++) {
            elements.get(i)->setRenderPositionByOffset(xLocation,yLocation);
        }
    }

    // Override render to include child elements
    virtual void render() override {
        if (visible) {
            // Render all child elements
            for (int i = 0; i < elements.size(); i++) {
                if (elements.get(i)->isVisible()) 
                    elements.get(i)->render();
            }
        }
    }

    // Override OnClick_execute to handle clicks and forward to child elements
    virtual void OnClick_execute(Point point) override {
        if (OnClick_enable && OnClick_handler != nullptr) {
            OnClick_handler(point);
        }
        
        // Forward click events to child elements
        for (int i = 0; i < elements.size(); i++) {
            if (elements.get(i)->inRange(point)) {
                elements.get(i)->OnClick_execute(point);
            }
        }
    }

    // Add a child element
    void addElement(Element* element) {
        elements.add(element);
    }

    // Remove a child element
    void removeElement(Element* element) {
        // Iterate and remove the specified element
        for (int i = 0; i < elements.size(); i++) {
            if (elements.get(i) == element) {
                elements.remove(i);
                break;
            }
        }
    }

private:
    HyperList<Element*> elements; // List of child elements
};

#endif // ACTIVITY_HPP
