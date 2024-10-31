#ifndef TOUCHEVENT_HPP
#define TOUCHEVENT_HPP

#include "TGUI.hpp"

// Variables for tracking touch states
volatile bool touchPressed = false;
volatile uint16_t lastX = 0, lastY = 0;
volatile unsigned long pressStartTime = 0;
const unsigned long debounceDelay = 50;  // 50 ms debounce delay
const uint16_t dragThreshold = 5;  // Minimum movement in pixels to count as a drag


// ISR to handle touch press/release and debounce
void IRAM_ATTR touchISR() {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();

    // Debounce check
    if (interruptTime - lastInterruptTime > debounceDelay) {
        bool isTouched = digitalRead(TOUCH_PIN) == LOW;  // Adjust based on touch module's active state
        uint16_t x, y;

        if (isTouched) {
            // Register a press event
            touchPressed = true;
            pressStartTime = millis();
            tft.getTouch(&x, &y);  // Replace with your library's touch reading function
            lastX = x;
            lastY = y;
            onPress(x, y);
        } else if (touchPressed) {
            // Register a release event
            touchPressed = false;
            unsigned long pressDuration = millis() - pressStartTime;
            tft.getTouch(&x, &y);
            onRelease(x, y, pressDuration);
        }
    }
    lastInterruptTime = interruptTime;
}

// Main loop to handle drag events
void loop() {
    if (touchPressed) {
        uint16_t x, y;
        tft.getTouch(&x, &y);

        // Check if movement exceeds drag threshold
        if (abs(x - lastX) > dragThreshold || abs(y - lastY) > dragThreshold) {
            lastX = x;
            lastY = y;
            onDrag(x, y);
        }
    }
}


enum TouchStatus{
    ready,
    pressed,
    hold,
    drag,
    released,
    click,
    size
};

class Touch {
protected:
    TouchStatus status;
    ivec2 currentPosition, startPosition, endPosition;
    int counter;

    bool enable; // Flag to enable touch events

    Screen *screen;

public:
    // Constructors
    Touch(Screen *screen) {
        enable = false;
    }

    virtual void init(){
        // Configure touch interrupt pin and setup ISR
        pinMode(TOUCH_PIN, INPUT);
        attachInterrupt(digitalPinToInterrupt(TOUCH_PIN), touchISR, CHANGE);  // CHANGE triggers on both press and release
   
    }

    virtual void reset(){
        status = ready;
    }

    virtual void next() {
        if (status == TouchStatus::size) {
            reset();
        }
        
    }
};

#endif // TOUCHEVENT_HPP