#ifndef PUSHBUTTON_HPP
#define PUSHBUTTON_HPP

#include <Arduino.h>

// Enum for event types
enum PushbuttonStatus : byte {
    PRESS,
    RELEASE,
    PushbuttonStatus_size
};

using ISRpointer = void (*)();

class Pushbutton {
private:
    uint8_t pin;                   // Pin number
    uint32_t debounceThreshold;    // Debounce threshold in milliseconds or microseconds
    bool useMicros;                // Whether to use micros() instead of millis()
    ISRpointer isrPointer;         // Pointer to the ISR

    volatile uint32_t lastDebounceTime;   // Last debounce time
    volatile bool buttonState;           // Current button state
    volatile bool lastButtonState;       // Previous button state

    volatile uint32_t currentTime;       // Current time
    volatile PushbuttonStatus currentState; // Current state

    volatile bool _hasPressed;           // Flag for a press event
    volatile bool _hasReleased;          // Flag for a release event

    bool enablePress;                    // Enable press events
    bool enableRelease;                  // Enable release events

public:
    /**
     * Constructor
     * @param buttonPin Pin number for the pushbutton.
     * @param debounceMs Debounce threshold in milliseconds.
     * @param useMicros Whether to use micros() instead of millis().
     * @param handler Callback function for button events.
     */
    Pushbutton(uint8_t buttonPin, uint32_t debounceMs = 50, bool useMicros = false,
               ISRpointer isr = nullptr)
        : pin(buttonPin),
          debounceThreshold(debounceMs),
          useMicros(useMicros),
          isrPointer(isr),
          lastDebounceTime(0),
          buttonState(false),
          lastButtonState(false),
          currentTime(0),
          currentState(PushbuttonStatus_size),
          enablePress(false),
          enableRelease(false),
          _hasPressed(false),
          _hasReleased(false) {}
    
    ~Pushbutton() {
        detachInterrupt(digitalPinToInterrupt(pin));
    }

    /**
     * Initialize the pushbutton by configuring the pin and setting up the ISR.
     */
    void init() {
        pinMode(pin,INPUT_PULLUP);
        if (isrPointer) {
            attachInterrupt(digitalPinToInterrupt(pin), isrPointer, CHANGE);
        }
    }

    /**
     * Enable or disable press event handling.
     */
    void enablePressEvent(bool enable) {
        enablePress = enable;
    }

    /**
     * Enable or disable release event handling.
     */
    void enableReleaseEvent(bool enable) {
        enableRelease = enable;
    }

    /**
     * Check if a press event occurred.
     * Clears the internal flag after reading.
     */
    bool hasPressed() {
        bool state = _hasPressed;
        _hasPressed = false;
        return state;
    }

    /**
     * Check if a release event occurred.
     * Clears the internal flag after reading.
     */
    bool hasReleased() {
        bool state = _hasReleased;
        _hasReleased = false;
        return state;
    }

    /**
     * Get the current button state (pressed or not).
     */
    bool isPressed() const {
        return digitalRead(pin) == LOW;
    }

    /**
     * Get the current time based on millis() or micros().
     */
    uint32_t getTime() const {
        return useMicros ? micros() : millis();
    }

    /**
     * Interrupt Service Routine (ISR) for handling button events.
     * This method should be as fast as possible to avoid ISR overhead.
     */
    void handleInterrupt() IRAM_ATTR {
        bool currentButtonState = isPressed();
        currentTime = getTime();

        if (currentButtonState != lastButtonState) {
            if (currentTime - lastDebounceTime >= debounceThreshold) {
                lastDebounceTime = currentTime;

                if (currentButtonState && enablePress) {
                    _hasPressed = true;
                    currentState = PRESS;
                } else if (!currentButtonState && enableRelease && currentState == PRESS) {
                    _hasReleased = true;
                    currentState = RELEASE;
                }
            }
        }

        lastButtonState = currentButtonState;
    }
};

#endif // PUSHBUTTON_HPP
