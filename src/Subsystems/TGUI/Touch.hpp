#ifndef TOUCH_HPP
#define TOUCH_HPP

#include "TGUI.hpp"
#include "Screen.hpp"

class Touch {
protected:
    byte status; // Status of the touch event - TouchStatus: press, release, hold, drag

    TouchDragData dragData; // Data for the drag event - start, current, end positions

    volatile unsigned long lastTime; // Last time the button was pressed in milliseconds
    volatile unsigned long pressStartTime; // Time when the press started in milliseconds
    unsigned long holdThreshold = 2500; // Hold time in milliseconds
    float dragDistanceThreshold = 10.0f; // Drag distance in pixels

    byte enable; // Flags to enable touch events

    Screen *screen;

public:
    // Constructors
    Touch(Screen *screen) {
        beenTouched = 0;
        lastTime = 0;
        pressStartTime = 0;
        enable = false;
        flagTemp = enable;
    }

    virtual void enable(int enable){
        this->enable = enable;
    }

    virtual void reset(){
        status = TouchStatus::READY;
    }

    virtual void next(ivec2 point, bool isEdge, bool isTouched) {
        if (enable)
        {
            dragData.currentPosition = point;

            if (status >= TouchStatus::TouchStatus_size) {
                reset();
            }
            
            if (isEdge)
            {
                if (isTouched)
                {
                    status = TouchStatus::PRESS;
                    dragData.startPosition = dragData.currentPosition;
                    pressStartTime = millis();
                }
                else
                {
                    status = TouchStatus::RELEASE;
                    dragData.endPosition = point;
                }
            }
            else if (isTouched && status > TouchStatus::READY)
            {
                if (status == TouchStatus::PRESS && millis() - pressStartTime > holdThreshold)
                {
                    status = TouchStatus::HOLD;
                }
                if (distance(dragData.currentPosition, dragData.startPosition) > dragDistanceThreshold)
                {
                    if (status == TouchStatus::HOLD || status == TouchStatus::DRAG)
                    {
                        status = TouchStatus::DRAG;
                    }
                    else
                    {
                        status = TouchStatus::SWIPE;
                    }
                }
            }
            else
            {
                reset();
            }

            if (enable & (1 << status))
            {
                screen->executeTouch(point, TouchStatus(status));
            }
        }
    }
};

#endif // TOUCH_HPP