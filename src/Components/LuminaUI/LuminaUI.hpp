/**
 * @file LuminaUI.hpp
 * @brief Core definitions for LuminaUI: layout, touch handling, and UI element integration.
 *
 * Provides viewport management, touch status/data structures, and includes key sub-modules:
 * Elements, Activities, Screens, and Touch handling. Supports optional TFT_eSPI elements.
 */

 #ifndef LUMINAUI_HPP
 #define LUMINAUI_HPP
 
 #include <Arduino.h>
 
 #include "Utilities/ivec2.hpp"    ///< Integer 2D vector type
 #include "Utilities/vec2.hpp"     ///< Float 2D vector type
 #include "Utilities/HyperList.hpp"///< Generic doubly linked list
 #include "Components/Pushbutton/Pushbutton.hpp" ///< Debounced button handler
 
 /**
  * @brief Sentinel value for automatic origin or scale.
  *
  * Used to indicate that the UI layout system should compute
  * a default value for position or size.
  */
 const ivec2 LuminaUI_AUTO = ivec2(-1, -1);
 
 // --------------------- VIEWPORT ---------------------
 /**
  * @brief Defines a rectangular rendering area in 2D space.
  */
 struct Viewport {
     ivec2 position; ///< Top-left corner of the viewport
     ivec2 scale;    ///< Width (x) and height (y) of the viewport
 
     /**
      * @brief Construct a Viewport with position and size.
      * @param position Top-left corner (default 0,0).
      * @param scale    Width and height (default 0,0).
      */
     Viewport(ivec2 position = ivec2(0, 0), ivec2 scale = ivec2(0, 0))
       : position(position), scale(scale) {}
 
     /**
      * @brief Compute the center point of the viewport.
      * @return Center coordinates = position + scale/2.
      */
     ivec2 center() const {
         return position + scale / 2;
     }
 
     /**
      * @brief Determine whether a point lies within this viewport.
      * @param point Coordinates to test.
      * @return True if within [position, position+scale].
      */
     bool inRange(const ivec2 point) const {
         return (point.x >= position.x && point.x <= position.x + scale.x &&
                 point.y >= position.y && point.y <= position.y + scale.y);
     }
 
     /**
      * @brief Check if another viewport overlaps this one.
      * @param other Viewport to test.
      * @return True if any corner of other lies within this viewport.
      */
     bool inRange(const Viewport other) const {
         ivec2 e_scale = other.scale;
         ivec2 pLT = other.position;
         ivec2 pLB = other.position + ivec2(0, e_scale.y);
         ivec2 pRT = other.position + ivec2(e_scale.x, 0);
         ivec2 pRB = other.position + e_scale;
         return inRange(pLT) || inRange(pLB) || inRange(pRT) || inRange(pRB);
     }
 
     /**
      * @brief Offset position by padding amounts.
      * @param padding Array of {left, top, right, bottom} paddings.
      * @return New top-left after applying left/top padding.
      */
     ivec2 positionAfterPadding(const int padding[4]) const {
         return position + ivec2(padding[0], padding[1]);
     }
 
     /**
      * @brief Compute inner scale after removing padding.
      * @param padding Array of {left, top, right, bottom} paddings.
      * @return Reduced scale.
      */
     ivec2 scaleInsidePadding(const int padding[4]) const {
         return scale - ivec2(padding[0], padding[1]) - ivec2(padding[2], padding[3]);
     }
 
     /**
      * @brief Create a new viewport with padding applied.
      * @param padding Array of {left, top, right, bottom} paddings.
      * @return Viewport inset by padding.
      */
     Viewport afterPadding(const int padding[4]) const {
         return { positionAfterPadding(padding), scaleInsidePadding(padding) };
     }
 
     /**
      * @brief Compute intersection of two viewports.
      * @param other Viewport to clamp against.
      * @return Overlapping viewport or {0,0,0,0} if none.
      */
     Viewport clamp(const Viewport &other) const {
         ivec2 newPos = position.max(other.position);
         ivec2 newEnd = (position + scale).min(other.position + other.scale);
         ivec2 newScale = newEnd - newPos;
         if (newScale.x <= 0 || newScale.y <= 0) return Viewport{ivec2(0,0), ivec2(0,0)};
         return Viewport{ newPos, newScale };
     }
 
     /**
      * @brief String representation for debugging.
      * @return "Viewport: {Position: (x,y) Scale: (w,h)}".
      */
     String toString() const {
         return String("Viewport: {Position: ") + position.toString() +
                " Scale: " + scale.toString() + "}";
     }
 };
 
 // --------------------- TOUCH HANDLING ---------------------
 /**
  * @brief States of touch interaction.
  */
 enum TouchStatus : byte {
     TouchStatus_READY,   ///< No current touch
     TouchStatus_PRESS,   ///< Initial press detected
     TouchStatus_RELEASE, ///< Release event
     TouchStatus_HOLD,    ///< Sustained press
     TouchStatus_DRAG,    ///< Movement while pressed
     TouchStatus_size     ///< Count of statuses
 };
 
 /**
  * @brief String labels for TouchStatus values.
  */
 static const String TouchStatus_strings[] = {
     "READY", "PRESS", "RELEASE", "HOLD", "DRAG", "size"
 };
 
 // Enable flags for event types
 #define ENABLE_PRESS   (1 << TouchStatus_PRESS)
 #define ENABLE_RELEASE (1 << TouchStatus_RELEASE)
 #define ENABLE_HOLD    (1 << TouchStatus_HOLD)
 #define ENABLE_DRAG    (1 << TouchStatus_DRAG)
 #define ENABLE_ALL    ((1 << TouchStatus_size) - 1 & ~ (1 << TouchStatus_READY))
 
 /**
  * @brief Data for a touch event lifecycle.
  */
 struct TouchData {
     TouchStatus status;       ///< Current touch state
     ivec2 startPosition;      ///< Coordinates at initial press
     ivec2 currentPosition;    ///< Coordinates during drag
     ivec2 endPosition;        ///< Coordinates at release
 };
 
 /**
  * @brief Simplified drag data without status.
  */
 struct TouchDragData {
     ivec2 startPosition;      ///< Initial press position
     ivec2 currentPosition;    ///< Current drag position
     ivec2 endPosition;        ///< Release position
 };
 
 // --------------------- MODULE INCLUDES ---------------------
 #include "Element.hpp"  ///< Base UI element definitions
 #include "Activity.hpp" ///< Activity (app screen) abstractions
 #include "Screen.hpp"   ///< Screen management for multiple activities
 #include "Touch.hpp"    ///< Touch input processing
 
 #ifdef LUMINAUI_USE_TFT_ESPI
   #include "TFT_eSPI_Elements/TFT_eSPI_Elements.hpp" ///< Optional TFT_eSPI drivers
 #endif
 
 #endif // LUMINAUI_HPP 