/**
 * @file Activity.hpp
 * @brief Container class for managing and rendering multiple UI elements within a defined area.
 *
 * The Activity class extends Element to provide layout, rendering, and event propagation
 * for its child elements. It handles viewport clipping, touch event delegation, and child
 * element lifecycle management.
 */

 #ifndef ACTIVITY_HPP
 #define ACTIVITY_HPP
 
 #include "Element.hpp"
 #include "LuminaUI.hpp"
 
 /**
  * @class Activity
  * @brief Manages a group of Elements, handling layout, rendering, and touch events.
  *
  * Activity provides a rectangular area (viewport) where child elements are placed and
  * drawn. It propagates touch events to children, clips rendering to its viewport,
  * and updates child positions based on its own position and scale.
  */
 class Activity : public Element {
 public:
     HyperList<Element *> elements; ///< List of child element pointers
 
     /**
      * @brief Constructs an Activity with optional parameters.
      *
      * @param origin           Top-left corner of the Activity (default auto = 0,0).
      * @param scale            Width and height of the Activity (default auto = 0,0).
      * @param visible          Whether the Activity is initially visible.
      * @param renderAlways     If true, always render children each loop.
      * @param OnTouch_enable   Enable or disable touch events on the Activity.
      * @param OnTouch_handler  Callback for touch events on the Activity.
      */
     Activity(
         ivec2 origin = LuminaUI_AUTO,
         ivec2 scale = LuminaUI_AUTO,
         bool visible = true,
         bool renderAlways = true,
         bool OnTouch_enable = false,
         TouchEvent OnTouch_handler = nullptr)
       : Element(origin, ivec2(), scale, visible, renderAlways, OnTouch_enable, OnTouch_handler) {
         if (origin == LuminaUI_AUTO) this->origin = ivec2(0, 0);
         if (scale == LuminaUI_AUTO) this->scale = this->scale;
     }
 
     /**
      * @brief Constructs an Activity from an existing Element.
      *
      * Copies the base Element's position, size, visibility, and touch settings.
      * @param element Existing Element instance.
      */
     Activity(const Element &element)
       : Activity(element.origin, element.scale, element.visible,
                  element.renderAlways, element.OnTouch_enable, element.OnTouch_handler) {}
 
     /**
      * @brief Render the Activity and its visible children.
      *
      * Clamps drawing to the provided viewport, updates child viewports,
      * and calls render on each child in back-to-front order.
      * @param parentViewport Clipping region from parent.
      * @return Clamped Activity viewport.
      */
     virtual Viewport render(const Viewport &parentViewport) override {
         // Clip this Activity
         Viewport activityVP = Element::render(parentViewport);
         updateViewports();
         if (visible) {
             // Render children in insertion order (back to front)
             for (int i = 0; i < getElementCount(); ++i) {
                 Element* elem = getElement(i);
                 if (!elem) continue;
                 Viewport childVP = (elem->scale != LuminaUI_AUTO)
                     ? elem->getViewport() : activityVP;
                 if (elem->visible && elem->shouldRender() && activityVP.inRange(childVP)) {
                     childVP = childVP.clamp(activityVP);
                     elem->render(childVP);
                 }
             }
         }
         return activityVP;
     }
 
     /**
      * @brief Handle touch events and delegate to child elements.
      *
      * Calls base OnTouch, updates child viewports, then delivers touch to
      * the topmost child under the touch point. On RELEASE, resets others.
      * @param point Screen coordinates of touch.
      * @param touchStatus Type of touch event.
      */
     virtual void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override {
         Element::OnTouch_execute(point, touchStatus);
         updateViewports();
         bool handled = false;
         // Iterate children in reverse (front to back)
         for (int i = getElementCount() - 1; i >= 0; --i) {
             Element* elem = getElement(i);
             if (!elem || !elem->visible || !elem->OnTouch_enable || !elem->OnTouch_handler)
                 continue;
             if (!handled && elem->inRange(point)) {
                 elem->OnTouch_execute(point, touchStatus);
                 handled = true;
             }
             if (touchStatus == TouchStatus_RELEASE && !handled) {
                 // Notify READY status for unaffected children
                 elem->OnTouch_execute(elem->getPosition(), TouchStatus_READY);
             }
         }
     }
 
     /**
      * @brief Update child origins and scales relative to this Activity.
      *
      * Sets offset to this Activity's position, and defaults origin/scale if auto.
      */
     virtual void updateViewports() {
         for (int i = 0; i < getElementCount(); ++i) {
             Element* elem = getElement(i);
             if (!elem) continue;
             if (elem->origin == LuminaUI_AUTO) elem->origin = this->origin;
             elem->offset = this->getViewport().position;
             if (elem->scale == LuminaUI_AUTO) elem->scale = this->scale;
         }
     }
 
     /**
      * @brief Add a single child Element.
      * @param element Pointer to Element instance (non-null).
      */
     void addElement(Element* element) {
         if (element) elements.addend(element);
     }
 
     /**
      * @brief Add multiple children from an array.
      * @param array Pointer to Element* array.
      * @param size Number of elements in array.
      */
     void addElements(Element** array, int size) {
         if (array) elements.addFromArray(array, size);
     }
 
     /**
      * @brief Remove child by index.
      * @param index Index within elements list.
      */
     void removeElement(int index) {
         elements.remove(index);
     }
 
     /**
      * @brief Remove a specific child Element.
      * @param element Pointer to the Element to remove.
      */
     void removeElement(Element* element) {
         for (int i = 0; i < elements.size(); ++i) {
             if (getElement(i) == element) {
                 removeElement(i);
                 break;
             }
         }
     }
 
     /**
      * @brief Remove all children.
      */
     void clearElements() {
         elements.clear();
     }
 
     /**
      * @brief Get number of child elements.
      * @return Count of children.
      */
     int getElementCount() const {
         return elements.size();
     }
 
     /**
      * @brief Access child by index.
      * @param index Position in list.
      * @return Pointer to Element or nullptr.
      */
     Element* getElement(int index) {
         return (index >= 0 && index < getElementCount()) ? elements.get(index) : nullptr;
     }
 };
 
 #endif // ACTIVITY_HPP 