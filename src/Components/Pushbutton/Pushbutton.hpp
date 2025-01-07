#ifndef PUSHBUTTON_HPP
#define PUSHBUTTON_HPP

#include <Arduino.h>

// Enum for event types
enum EventType {
    PRESS,
    RELEASE
};

// Define the event handler type
using EventHandler = void (*)(EventType, uint32_t);

class Pushbutton {
public:
    /**
     * Constructor
     * @param buttonPin Pin number for the pushbutton.
     * @param debounceMs Debounce threshold in milliseconds.
     * @param handler Callback function for button events.
     */
    Pushbutton(uint8_t buttonPin, uint32_t debounceMs = 50, EventHandler handler = nullptr)
        : pin(buttonPin),
          debounceThreshold(debounceMs),
          eventHandler(handler),
          lastDebounceTime(0),
          buttonState(false),
          lastButtonState(false),
          useMicros(false),
          enablePress(true),
          enableRelease(true),
          hasPressed(false) {}

    /**
     * Initialize the pushbutton by configuring the pin and setting up the ISR.
     */
    void init() {
        pinMode(pin, INPUT_PULLUP);
        attachInterruptArg(digitalPinToInterrupt(pin), Pushbutton::isrHandler, this, CHANGE);
    }

    /**
     * Enable or disable press and release events.
     */
    void enableAllEvents(bool enable) {
        enablePress = enable;
        enableRelease = enable;
    }

    void enablePressEvent(bool enable) {
        enablePress = enable;
    }

    void enableReleaseEvent(bool enable) {
        enableRelease = enable;
    }

    /**
     * Sets whether to use microseconds for timing instead of milliseconds.
     */
    void setUseMicros(bool enable) {
        useMicros = enable;
    }

    /**
     * Checks if the button was pressed (read and clear flag).
     */
    bool readHasPressed() {
        bool state = hasPressed;
        hasPressed = false;
        return state;
    }

    /**
     * Checks the current button state (useful for polling if needed).
     * @return true if the button is pressed, false otherwise.
     */
    bool isPressed() const {
        return digitalRead(pin) == LOW;
    }

private:
    uint8_t pin;                   // Pin number
    uint32_t debounceThreshold;    // Debounce threshold in milliseconds or microseconds
    volatile uint32_t lastDebounceTime; // Last debounce time
    volatile bool buttonState;     // Current button state
    volatile bool lastButtonState; // Previous button state
    volatile bool hasPressed;      // Flag to indicate a press event occurred
    EventHandler eventHandler;     // Callback for events
    bool useMicros;                // Whether to use micros() instead of millis()
    bool enablePress;              // Enable press events
    bool enableRelease;            // Enable release events

    /**
     * Static ISR handler. This function is called by the interrupt and delegates
     * handling to the instance of the class.
     */
    static void IRAM_ATTR isrHandler(void *arg) {
        Pushbutton *self = static_cast<Pushbutton *>(arg);
        self->handleInterrupt();
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
                    hasPressed = true;
                    if (eventHandler) {
                        eventHandler(PRESS, currentTime - lastDebounceTime);
                    }
                } else if (!currentButtonState && enableRelease) {
                    if (eventHandler) {
                        eventHandler(RELEASE, currentTime - lastDebounceTime);
                    }
                }
            }
        }
        lastButtonState = currentButtonState;
    }
};

#endif // PUSHBUTTON_HPP
