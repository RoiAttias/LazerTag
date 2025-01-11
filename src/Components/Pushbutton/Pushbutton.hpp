#ifndef PUSHBUTTON_HPP
#define PUSHBUTTON_HPP

#include <Arduino.h>

// Enum for event types
enum PushbuttonStatus : byte {
    PRESS,
    RELEASE,
    PushbuttonStatus_size
};

// Define the event handler type
using PushbuttonEventHandler = void (*)(PushbuttonStatus, uint32_t);

class Pushbutton;
static void IRAM_ATTR PushbuttonISR(void *arg);

class Pushbutton {
private:
    uint8_t pin;                   // Pin number
    uint32_t debounceThreshold;    // Debounce threshold in milliseconds or microseconds
    bool useMicros;                // Whether to use micros() instead of millis()
    PushbuttonEventHandler eventHandler;     // Callback for events
    volatile uint32_t lastDebounceTime; // Last debounce time
    volatile bool buttonState;     // Current button state
    volatile bool lastButtonState; // Previous button state
    volatile bool _hasPressed;      // Flag to indicate a press event occurred
    volatile bool _hasReleased;    // Flag to indicate a release event occurred
    bool enablePress;              // Enable press events
    bool enableRelease;            // Enable release events

public:
    /**
     * Constructor
     * @param buttonPin Pin number for the pushbutton.
     * @param debounceMs Debounce threshold in milliseconds.
     * @param useMicros Whether to use micros() instead of millis().
     * @param handler Callback function for button events.
     */
    Pushbutton(uint8_t buttonPin, uint32_t debounceMs = 50, bool useMicros = false, PushbuttonEventHandler handler = nullptr)
        : pin(buttonPin),
          debounceThreshold(debounceMs),
          useMicros(useMicros),
          eventHandler(handler),
          lastDebounceTime(0),
          buttonState(false),
          lastButtonState(false),
          enablePress(false),
          enableRelease(false),
          _hasPressed(false),
          _hasReleased(false) {}

    /**
     * Initialize the pushbutton by configuring the pin and setting up the ISR.
     */
    void init() {
        pinMode(pin, INPUT_PULLUP);
        attachISR();
    }

    /**
     * Attach the ISR to the pin.
     */
    void attachISR()
    {
        attachInterruptArg(digitalPinToInterrupt(pin), PushbuttonISR, this, CHANGE);
    }

    /**
     * Detach the ISR from the pin.
     */
    void detachISR()
    {
        detachInterrupt(digitalPinToInterrupt(pin));
    }

    int8_t getPin() const {
        return pin;
    }

    void enablePressEvent(bool enable) {
        enablePress = enable;
    }

    void enableReleaseEvent(bool enable) {
        enableRelease = enable;
    }

    /**
     * Checks if the button was pressed (read and clear flag).
     */
    bool hasPressed() {
        bool state = _hasPressed;
        _hasPressed = false;
        return state;
    }

    /**
     * Checks if the button was released (read and clear flag).
     */
    bool hasReleased() {
        bool state = _hasReleased;
        _hasReleased = false;
        return state;
    }

    /**
     * Checks the current button state (useful for polling if needed).
     * @return true if the button is pressed, false otherwise.
     */
    bool isPressed() const {
        return digitalRead(pin) == LOW;
    }

    /**
     * Handles the interrupt. This method is responsible for debouncing and
     * invoking the event handler if necessary.
     */
    void IRAM_ATTR handleInterrupt() {
        uint32_t currentTime = useMicros ? micros() : millis();
        bool currentButtonState = isPressed();

        if (currentButtonState != lastButtonState) {
            if (currentTime - lastDebounceTime >= debounceThreshold) {
                buttonState = currentButtonState;
                lastDebounceTime = currentTime;

                if (currentButtonState && enablePress) {
                    _hasPressed = true;
                    if (eventHandler) {
                        eventHandler(PRESS, currentTime - lastDebounceTime);
                    }
                } else if (!currentButtonState && enableRelease) {
                    _hasReleased = true;
                    if (eventHandler) {
                        eventHandler(RELEASE, currentTime - lastDebounceTime);
                    }
                }
            }
        }
        lastButtonState = currentButtonState;
    }    
};

/**
 * Static ISR handler. This function is called by the interrupt and delegates
 * handling to the instance of the class.
 */
static void IRAM_ATTR PushbuttonISR(void *arg) {
    Pushbutton *self = static_cast<Pushbutton *>(arg);
    self->detachISR();
    self->handleInterrupt();
    self->attachISR();
}

#endif // PUSHBUTTON_HPP
