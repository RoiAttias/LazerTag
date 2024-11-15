#include <Arduino.h>

class Pushbutton {
private:
    uint8_t pin;                     // Pin number the button is connected to
    uint32_t lastDebounceTime;       // Timestamp of the last valid state change
    uint32_t debounceThreshold;      // Debounce threshold in milliseconds
    void (*pressEventHandler)();     // Function pointer for press event handler
    void (*releaseEventHandler)();   // Function pointer for release event handler
    bool buttonState;                // The last stable state of the button (pressed/released)
    bool currentButtonState;         // The current state of the button (pressed/released)
    bool pressEnabled;               // Flag to enable/disable press events
    bool releaseEnabled;             // Flag to enable/disable release events

public:
    // Constructor to initialize the button with its pin and debounce threshold
    Pushbutton(uint8_t buttonPin, uint32_t debounceMs = 50)
        : pin(buttonPin), debounceThreshold(debounceMs), lastDebounceTime(0),
          pressEventHandler(nullptr), releaseEventHandler(nullptr),
          buttonState(false), currentButtonState(false),
          pressEnabled(true), releaseEnabled(true) {}

    /**
     * Initialize the button with optional event handlers and settings
     * 
     * @param pressHandler      Optional function to handle press events
     * @param enablePress       Enable/disable press event handling
     * @param releaseHandler    Optional function to handle release events
     * @param enableRelease     Enable/disable release event handling
     */
    void init(void (*pressHandler)() = nullptr, bool enablePress = false,
              void (*releaseHandler)() = nullptr, bool enableRelease = false) {
        // Assign event handlers if provided
        if (pressHandler) {
            pressEventHandler = pressHandler;
            pressEnabled = enablePress;
        }
        if (releaseHandler) {
            releaseEventHandler = releaseHandler;
            releaseEnabled = enableRelease;
        }

        // Initialize button state variables
        buttonState = false;
        currentButtonState = false;

        // Configure the pin as INPUT_PULLUP for the button
        pinMode(pin, INPUT_PULLUP);

        // Attach the interrupt for state changes on the button pin
        attachInterrupt(digitalPinToInterrupt(pin), [this]() { this->handleInterrupt(); }, CHANGE);
    }

    /**
     * Set a custom press event handler
     * 
     * @param handler Function to handle press events
     */
    void setPressEventHandler(void (*handler)()) {
        pressEventHandler = handler;
    }

    /**
     * Set a custom release event handler
     * 
     * @param handler Function to handle release events
     */
    void setReleaseEventHandler(void (*handler)()) {
        releaseEventHandler = handler;
    }

    /**
     * Enable or disable the press event
     * 
     * @param enable Boolean to enable or disable press event handling
     */
    void enablePressEvent(bool enable) {
        pressEnabled = enable;
    }

    /**
     * Enable or disable the release event
     * 
     * @param enable Boolean to enable or disable release event handling
     */
    void enableReleaseEvent(bool enable) {
        releaseEnabled = enable;
    }

    /**
     * Interrupt handler for button state changes
     * This function debounces the button and triggers events if applicable.
     */
    void handleInterrupt() {
        uint32_t currentTime = millis();             // Get the current time in milliseconds
        currentButtonState = digitalRead(pin) == LOW; // Read the button state (LOW means pressed)

        // Check if enough time has passed since the last state change (debounce check)
        if ((currentTime - lastDebounceTime) > debounceThreshold) {
            // Check for state transitions and trigger events if enabled
            if (currentButtonState && !buttonState && pressEnabled && pressEventHandler) {
                pressEventHandler(); // Call the press event handler
            } else if (!currentButtonState && buttonState && releaseEnabled && releaseEventHandler) {
                releaseEventHandler(); // Call the release event handler
            }

            // Update the stable button state and debounce timestamp
            buttonState = currentButtonState;
            lastDebounceTime = currentTime;
        }
    }
};