#ifndef TOUCH_HPP
#define TOUCH_HPP

#include "LuminaUI.hpp"

class Touch {
protected:
    TouchStatus status; // Status of the touch event - TouchStatus: press, release, hold, drag

    TouchDragData dragData;

    volatile unsigned long pressStartTime; // Time when the press started in milliseconds
    ivec2 lastPoint; // Last point of touch

    const unsigned long pressDebounceThreshold = 150; // Debounce time in milliseconds
    const float dragDistanceThreshold = 25.0f; // Drag distance in pixels

    byte enable; // Flags to enable touch events

    Screen *screen;

public:
    // Constructors
    Touch(Screen *screen) : screen(screen), pressStartTime(0), 
        lastPoint(ivec2(0, 0)), enable(0) {}

    virtual void init(int enable){
        this->enable = enable;
        reset();
    }

    virtual void reset(){
        status = TouchStatus::TouchStatus_READY;
    }

    virtual ivec2 getLastPoint() {
        return lastPoint;
    }
    
    virtual void next(ivec2 point, bool isEdge, bool isTouched) {
        if (enable) {
            bool execute = false;
            unsigned long currentTime = millis();
            // Update the current touch position
            dragData.currentPosition = point;
            // If the status is invalid, reset
            if (status >= TouchStatus::TouchStatus_size) {
                reset();
            }
            
            // Handle the edge (press/release) events
            if (isEdge) {
                if (isTouched && status != TouchStatus::TouchStatus_PRESS && currentTime - pressStartTime > pressDebounceThreshold) {
                    // Handle press event
                    status = TouchStatus::TouchStatus_PRESS;
                    dragData.startPosition = dragData.currentPosition;
                    pressStartTime = currentTime;
                    execute = true;
                } else if (!isTouched && status != TouchStatus::TouchStatus_RELEASE) {
                    // Handle release event
                    status = TouchStatus::TouchStatus_RELEASE;
                    dragData.endPosition = dragData.currentPosition;
                    execute = true;
                } else {
                    reset();
                }
            } 
            // Handle other touch interactions (hold, drag, swipe)
            else if (isTouched && status > TouchStatus::TouchStatus_READY) {
                // Check if it's a hold event
                if (status == TouchStatus::TouchStatus_PRESS || status == TouchStatus::TouchStatus_HOLD) {
                    status = TouchStatus::TouchStatus_HOLD;
                    execute = true;
                }
                
                // Check for drag or swipe based on movement
                if (vec2(dragData.currentPosition).distanceTo(dragData.startPosition) > dragDistanceThreshold) {
                    if (status == TouchStatus::TouchStatus_HOLD || status == TouchStatus::TouchStatus_DRAG) {
                        status = TouchStatus::TouchStatus_DRAG;
                        execute = true;
                    }
                }
            }
            // Reset touch status when not touched
            else {
                reset();
            }
            
            // If the status is press or release and not an edge event, return
            if ((status == TouchStatus::TouchStatus_PRESS || status == TouchStatus::TouchStatus_RELEASE) && !isEdge) {
                return;
            }

            // If the current status is enabled, trigger the screen's touch event
            if (enable & (1 << status) && execute) {
                screen->executeTouch(point, status);
                if (status == TouchStatus::TouchStatus_RELEASE) {
                    reset();
                }
            }
        }
    }

};

#endif // TOUCH_HPP