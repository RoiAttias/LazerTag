/**
 * @file Touch.hpp
 * @brief Processes raw touch input into higher-level touch events and delegates to Screen.
 *
 * The Touch class handles press, release, hold, and drag detection based on
 * input coordinates and edge transitions. It debounces press events and measures
 * drag distance thresholds. When enabled, it forwards events to the associated Screen.
 */

 #ifndef TOUCH_HPP
 #define TOUCH_HPP
 
 #include "LuminaUI.hpp"
 
 /**
  * @class Touch
  * @brief Converts raw touch signals into TouchStatus events and invokes Screen callbacks.
  *
  * Maintains state for press timing, debouncing, and drag detection. Supports configurable
  * thresholds for debounce and minimum drag distance. Use init() to configure enabled events.
  */
 class Touch {
 protected:
     TouchStatus status;                ///< Current touch phase (PRESS, HOLD, etc.)
     TouchDragData dragData;            ///< Data for drag events
 
     volatile unsigned long pressStartTime; ///< Timestamp when last press occurred
     ivec2 lastPoint;                   ///< Last reported touch coordinates
 
     const unsigned long pressDebounceThreshold = 150; ///< ms to debounce press
     const float dragDistanceThreshold = 25.0f;        ///< px to detect drag
 
     byte enable;                       ///< Bitmask of enabled TouchStatus events
     Screen* screen;                    ///< Screen instance to receive events
 
 public:
     /**
      * @brief Construct Touch processor linked to a Screen.
      * @param screen Pointer to Screen for event dispatch.
      */
     Touch(Screen* screen)
       : status(TouchStatus_READY), dragData(),
         pressStartTime(0), lastPoint(0, 0), enable(0), screen(screen) {}
 
     /**
      * @brief Initialize touch event handling.
      * @param enableMask Bitmask of enabled TouchStatus events.
      */
     virtual void init(int enableMask) {
         enable = enableMask;
         reset();
     }
 
     /**
      * @brief Reset to READY state and clear timing data.
      */
     virtual void reset() {
         status = TouchStatus_READY;
     }
 
     /**
      * @brief Get last known touch point.
      * @return Coordinates of last touch.
      */
     virtual ivec2 getLastPoint() const {
         return lastPoint;
     }
     
     /**
      * @brief Process the next raw touch input.
      *
      * @param point     Current touch coordinates.
      * @param isEdge    True on press/release edge transitions.
      * @param isTouched True if touch is active (pressed down).
      */
     virtual void next(ivec2 point, bool isEdge, bool isTouched) {
         if (!enable) return;
         bool shouldDispatch = false;
         unsigned long now = millis();
         dragData.currentPosition = point;
 
         // Validate status
         if (status >= TouchStatus_size) reset();
 
         // Edge events: press/release
         if (isEdge) {
             if (isTouched && status != TouchStatus_PRESS && now - pressStartTime > pressDebounceThreshold) {
                 status = TouchStatus_PRESS;
                 dragData.startPosition = point;
                 pressStartTime = now;
                 shouldDispatch = true;
             } else if (!isTouched && status != TouchStatus_RELEASE) {
                 status = TouchStatus_RELEASE;
                 dragData.endPosition = point;
                 shouldDispatch = true;
             } else {
                 reset();
             }
         }
         // Hold and drag detection
         else if (isTouched && status > TouchStatus_READY) {
             if (status == TouchStatus_PRESS || status == TouchStatus_HOLD) {
                 status = TouchStatus_HOLD;
                 shouldDispatch = true;
             }
             if (vec2(dragData.currentPosition).distanceTo(dragData.startPosition) > dragDistanceThreshold) {
                 if (status == TouchStatus_HOLD || status == TouchStatus_DRAG) {
                     status = TouchStatus_DRAG;
                     shouldDispatch = true;
                 }
             }
         } else {
             reset();
         }
 
         // Only dispatch edge events on edges
         if ((status == TouchStatus_PRESS || status == TouchStatus_RELEASE) && !isEdge)
             return;
 
         // Dispatch if enabled and flagged
         if ((enable & (1 << status)) && shouldDispatch) {
             screen->executeTouch(point, status);
             if (status == TouchStatus_RELEASE) reset();
         }
     }
 };
 
 #endif // TOUCH_HPP 