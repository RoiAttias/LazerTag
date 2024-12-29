#ifndef TOUCH_HPP
#define TOUCH_HPP

#include "TGUI.hpp"
#include "Screen.hpp"

class Touch {
protected:
    int status;
    ivec2 currentPosition, startPosition, endPosition;
    int counter;

    volatile bool beenTouched;
    volatile unsigned long lastTime;
    volatile unsigned long pressStartTime;

    //unsigned long holdThreshold = 1000;
    //float dragDistanceThreshold = 30.0f;

    //bool enable; 
    bool enable; // Flags to enable touch events
    bool flagTemp;

    Screen *screen;

public:
    // Constructors
    Touch(Screen *screen) {
        beenTouched = false;
        lastTime = 0;
        pressStartTime = 0;
        enable = false;
        flagTemp = enable;
    }

    virtual void init(int enable){
        this->enable = enable;
    }

    virtual void reset(){
        flagTemp = enable;
        enable = false;
        beenTouched = false;
        status = ready;

        enable = flagTemp;
    }

    virtual void next(ivec2 point, bool isTouched) {
        if (!enable)
        {
            return;
        }
        
        if (status == TouchStatus::TouchStatus_size) {
            reset();
        }
        else if(isTouched != beenTouched)
        {
            status++;
            screen->executeTouch(point,TouchStatus(status));
        }
        beenTouched = isTouched;
    }
};

#endif // TOUCH_HPP