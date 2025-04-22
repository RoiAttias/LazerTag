/**
 * @file Element.hpp
 * @brief Base class for all GUI elements in LuminaUI.
 *
 * Manages positioning, sizing, visibility, and touch event handling.
 * Provides virtual methods for rendering and range checking.
 */

 #ifndef ELEMENT_HPP
 #define ELEMENT_HPP
 
 #include "LuminaUI.hpp"
 
 /**
  * @brief Function signature for touch event handlers.
  * @param point Coordinates of the touch event.
  * @param status Type of touch event (PRESS, RELEASE, etc.).
  */
 using TouchEvent = void(*)(ivec2 point, TouchStatus status);
 
 /**
  * @class Element
  * @brief Abstract base for all UI elements.
  *
  * Provides core properties: origin, offset, scale, visibility,
  * and touch event support. Derived classes must implement render().
  */
 class Element {
 protected:
     bool _shouldRender; ///< Internal flag indicating pending render
 
 public:
     ivec2 origin;               ///< Programmer-specified origin position
     ivec2 offset;               ///< Container-specified offset
     ivec2 scale;                ///< Width and height of the element
     bool visible;               ///< Whether the element is visible
     bool renderAlways;          ///< If true, always render regardless of _shouldRender
 
     bool OnTouch_enable;        ///< Enable touch events for this element
     TouchEvent OnTouch_handler; ///< Touch event callback function
 
     /**
      * @brief Constructs an Element with full parameter control.
      * @param origin       Initial origin (LuminaUI_AUTO for default 0,0)
      * @param offset       Initial offset (LuminaUI_AUTO for default 0,0)
      * @param scale        Size of the element (LuminaUI_AUTO for default 0,0)
      * @param visible      Initial visibility state
      * @param renderAlways Always render if true
      * @param OnTouch_enable Enable touch event handling
      * @param OnTouch_handler Callback for touch events
      */
     Element(
         ivec2 origin = LuminaUI_AUTO,
         ivec2 offset = LuminaUI_AUTO,
         ivec2 scale = LuminaUI_AUTO,
         bool visible = true,
         bool renderAlways = true,
         bool OnTouch_enable = false,
         TouchEvent OnTouch_handler = nullptr)
         : origin(origin), offset(offset), scale(scale), visible(visible),
           renderAlways(renderAlways),
           OnTouch_enable(OnTouch_enable), OnTouch_handler(OnTouch_handler),
           _shouldRender(true) {
         if (origin == LuminaUI_AUTO) origin = ivec2(0, 0);
         if (offset == LuminaUI_AUTO) offset = ivec2(0, 0);
         if (scale == LuminaUI_AUTO) scale = ivec2(0, 0);
     }
 
     /**
      * @brief Constructs an Element by specifying individual coordinates and dimensions.
      */
     Element(
         int xOrigin, int yOrigin,
         int xOffset, int yOffset,
         int width, int height,
         bool visible = true,
         bool renderAlways = false,
         bool OnTouch_enable = false,
         TouchEvent OnTouch_handler = nullptr)
       : Element(ivec2(xOrigin, yOrigin), ivec2(xOffset, yOffset), ivec2(width, height),
                 visible, renderAlways, OnTouch_enable, OnTouch_handler) {}
 
     virtual ~Element() = default; ///< Default destructor
 
     /**
      * @brief Get the computed position (origin + offset).
      * @return The absolute position of the element.
      */
     virtual ivec2 getPosition() const { return origin + offset; }
 
     /**
      * @brief Get the size of the element.
      * @return Width and height vector.
      */
     virtual ivec2 getSize() const { return scale; }
 
     /**
      * @brief Get the viewport rectangle for this element.
      * @return Viewport covering the element's area.
      */
     virtual Viewport getViewport() const { return Viewport(getPosition(), scale); }
 
     /**
      * @brief Determine if this element should be rendered.
      * @return True if a render is pending or always render is enabled.
      */
     bool shouldRender() const { return _shouldRender || renderAlways; }
 
     /**
      * @brief Mark the element as needing rendering.
      */
     void callRender() { _shouldRender = true; }
 
     /**
      * @brief Render the element within a given viewport.
      *
      * Resets the render flag and returns the clamped viewport.
      * @param parentViewport The parent clipping viewport.
      * @return The element's viewport clipped to parentViewport.
      */
     virtual Viewport render(const Viewport &parentViewport) {
         _shouldRender = false;
         return getViewport().clamp(parentViewport);
     }
 
     /**
      * @brief Check if a point lies within this element.
      * @param point Point in screen coordinates.
      * @return True if point is inside element bounds.
      */
     virtual bool inRange(const ivec2 point) const {
         return getViewport().inRange(point);
     }
 
     /**
      * @brief Check if coordinates lie within this element.
      */
     virtual bool inRange(int x, int y) const { return inRange(ivec2(x, y)); }
 
     /**
      * @brief Check if another element overlaps this element.
      */
     virtual bool inRange(const Element *other) const {
         return getViewport().inRange(other->getViewport());
     }
 
     /**
      * @brief Enable or disable touch event responses.
      */
     virtual void OnTouch_setEnable(bool enable) { OnTouch_enable = enable; }
 
     /**
      * @brief Set the touch event callback function.
      */
     virtual void OnTouch_setHandler(TouchEvent handler) { OnTouch_handler = handler; }
 
     /**
      * @brief Execute touch handler if enabled and point is in range.
      * @param point Location of touch.
      * @param status Type of touch event.
      */
     virtual void OnTouch_execute(ivec2 point, TouchStatus status) {
         if (OnTouch_enable && OnTouch_handler && inRange(point) && status < TouchStatus_size) {
             OnTouch_handler(point, status);
         }
     }
 };
 
 #endif // ELEMENT_HPP 