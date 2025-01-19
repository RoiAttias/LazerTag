#ifndef TOUCH_HPP
#define TOUCH_HPP

#include "Screen.hpp"

class Touch {
protected:
    TouchStatus status; // Status of the touch event - TouchStatus: press, release, hold, drag

    TouchDragData dragData; // Data for the drag event - start, current, end positions

    volatile unsigned long lastTime; // Last time the button was pressed in milliseconds
    volatile unsigned long pressStartTime; // Time when the press started in milliseconds
    ivec2 lastPoint; // Last point of touch
    unsigned long holdThreshold = 2500; // Hold time in milliseconds
    float dragDistanceThreshold = 10.0f; // Drag distance in pixels

    byte enable; // Flags to enable touch events

    Screen *screen;

public:
    // Constructors
    Touch(Screen *screen) {
        lastTime = 0;
        pressStartTime = 0;
        lastPoint = ivec2(0, 0);
        enable = false;
    }

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
        // Update the current touch position
        dragData.currentPosition = point;

        // If the status is invalid, reset
        if (status >= TouchStatus::TouchStatus_size) {
            reset();
        }

        // Handle the edge (press/release) events
        if (isEdge) {
            if (isTouched) {
                // Handle press event
                status = TouchStatus::TouchStatus_PRESS;
                dragData.startPosition = dragData.currentPosition;
                pressStartTime = millis();
            } else {
                // Handle release event
                status = TouchStatus::TouchStatus_RELEASE;
                dragData.endPosition = dragData.currentPosition;
            }
        } 
        // Handle other touch interactions (hold, drag, swipe)
        else if (isTouched && status > TouchStatus::TouchStatus_READY) {
            // Check if it's a hold event
            if (status == TouchStatus::TouchStatus_PRESS && millis() - pressStartTime > holdThreshold) {
                status = TouchStatus::TouchStatus_HOLD;
            }

            // Check for drag or swipe based on movement
            if (distance(dragData.currentPosition, dragData.startPosition) > dragDistanceThreshold) {
                if (status == TouchStatus::TouchStatus_HOLD || status == TouchStatus::TouchStatus_DRAG) {
                    status = TouchStatus::TouchStatus_DRAG;
                } else {
                    status = TouchStatus::TouchStatus_SWIPE;
                }
            }
        } 
        // Reset touch status when not touched
        else {
            reset();
        }
        Serial.printf("Point: %d, %d\n", point.x, point.y);
        Serial.printf("Touch Status: %d\n", status);
        // If the current status is enabled, trigger the screen's touch event
        if (enable & (1 << status)) {
            //screen->executeTouch(point, status);
            screen->executeTouch(ivec2(50,40), TouchStatus::TouchStatus_PRESS);
        }
    }
}

};

#endif // TOUCH_HPP