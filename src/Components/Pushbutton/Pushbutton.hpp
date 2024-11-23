#ifndef PUSHBUTTON_HPP
#define PUSHBUTTON_HPP

#include <Arduino.h>

class Pushbutton
{
public:
    // Enum for event types
    enum EventType
    {
        PRESS,
        RELEASE,
        COUNTER,
        HOLD
    };

    // Define the handler function type
    using EventHandler = void (*)(EventType, uint32_t timeSincePress);

private:
    // Pin configuration and debounce/hold timings
    const uint8_t pin;     // Pin number the button is connected to
    const bool microsMode; // Flag to determine whether to use micros() or millis()

    uint32_t debounceThreshold; // Debounce threshold in milliseconds/microseconds
    uint32_t holdThreshold;     // Hold threshold in milliseconds/microseconds

    // Button state tracking
    volatile bool buttonState;           // Last stable state of the button (pressed/released)
    volatile bool currentButtonState;    // Current state of the button (pressed/released)
    volatile uint32_t lastDebounceTime;  // Timestamp of the last valid state change
    volatile uint32_t holdStartTime;     // Timestamp when the button hold began
    volatile uint32_t pressStartTime;    // Timestamp when the button was last pressed
    volatile uint32_t timeSincePress;    // Time elapsed since the button was last pressed
    volatile bool counterShouldOverride; // Flag to override unnecessary events
    volatile bool holdDidFirst;          // Flag to track the first hold event

    // Event handler and flags
    EventHandler eventHandler;    // Unified event handler
    volatile bool allEnabled;     // Enable/disable all events
    volatile bool pressEnabled;   // Enable/disable press events
    volatile bool releaseEnabled; // Enable/disable release events
    volatile bool counterEnabled; // Enable/disable counter events
    volatile bool holdEnabled;    // Enable/disable hold events

    // Counter and Hold settings
    volatile int counter;     // Current counter value
    int counterTarget;        // Target value for the counter
    bool counterOverrideMode; // Flag for overriding counter behavior
    bool counterReleasedMode; // Flag for triggering counter on release

    uint32_t holdRepeatDelay; // Time interval for repeated hold events
    bool holdRepeatMode;      // Flag for enabling repeated hold events

    /**
     * Get the current time in the selected mode (micros or millis).
     * @return Current time in milliseconds or microseconds.
     */
    inline uint32_t getTime() const
    {
        return microsMode ? micros() : millis();
    }

public:
    /**
     * Constructor to initialize the button with its pin and timing settings.
     * @param buttonPin         Pin number the button is connected to.
     * @param debounceMs        Debounce threshold in milliseconds/microseconds.
     * @param holdMs            Hold threshold in milliseconds/microseconds.
     * @param useMicros         Use micros() instead of millis() for timing.
     */
    Pushbutton(uint8_t buttonPin, uint32_t debounceMs = 50, bool useMicros = false)
        : pin(buttonPin), microsMode(useMicros), debounceThreshold(debounceMs), holdThreshold(3000),
          buttonState(false), currentButtonState(false), lastDebounceTime(0), holdStartTime(0), pressStartTime(0),
          timeSincePress(0), counterShouldOverride(false), holdDidFirst(false), eventHandler(nullptr),
          pressEnabled(false), releaseEnabled(false), counterEnabled(false), holdEnabled(false),
          counter(-1), counterTarget(0), counterOverrideMode(false), counterReleasedMode(false),
          holdRepeatDelay(0), holdRepeatMode(false) {}

    /**
     * Initialize the button with unified event handler and settings.
     * @param handler Unified event handler function.
     * @param enableAll Boolean to enable/disable all events.
     */
    void init(EventHandler handler = nullptr, bool enableAll = true)
    {
        eventHandler = handler;
        pinMode(pin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pin), [this]()
                        { handleInterrupt(); }, CHANGE);
        enableAllEvents(enableAll);
    }

    /**
     * Check if the button is currently pressed.
     * @return True if the button is pressed (LOW state), false otherwise.
     */
    inline bool isPressed() const
    {
        return digitalRead(pin) == LOW; // LOW state indicates the button is pressed.
    }

    /**
     * Check if the button is currently released (not pressed).
     * @return True if the button is released (HIGH state), false otherwise.
     */
    inline bool isReleased() const
    {
        return !isPressed(); // Opposite of pressed.
    }

    /**
     * Reset the counter to its initial target value or update it with a new target.
     * @param newTarget Optional new target value for the counter.
     *                  If not provided, the counter resets to the current target.
     */
    void resetCounter(int newTarget = -1)
    {
        // Update the counter target if a new target is provided
        if (newTarget >= 0)
        {
            counterTarget = newTarget;
        }

        // Reset the counter to the target value
        counter = counterTarget;
    }

    // Setters
    /**
     * Set parameters for the counter event.
     * @param target Target value for the counter.
     * @param overrideMode Enable/disable counter override mode.
     * @param releasedMode Enable triggering counter on release.
     */
    void setCounterEvent(int target, bool overrideMode = false, bool releasedMode = false)
    {
        enableCounterEvent(false);
        counterOverrideMode = overrideMode;
        counterReleasedMode = releasedMode;
        resetCounter(target);
    }

    /**
     * Set parameters for the hold event.
     * @param threshold Threshold for hold duration in milliseconds/microseconds.
     * @param repeatMode Enable/disable repeated hold events.
     * @param repeatDelay Time interval for repeated hold events.
     */
    void setHoldEvent(uint32_t threshold = 3000, bool repeatMode = false, uint32_t repeatDelay = 10)
    {
        enableHoldEvent(false);
        holdThreshold = threshold;
        holdRepeatMode = repeatMode;
        holdRepeatDelay = repeatDelay;
    }

    // Enables
    /**
     * Enable/disable all events.
     * @param enable Boolean to enable/disable all events.
     */
    void enableAllEvents(bool enable)
    {
        allEnabled = enable;
    }

    /**
     * Enable/disable press events.
     * @param enable Boolean to enable/disable press events.
     */
    void enablePressEvent(bool enable)
    {
        pressEnabled = enable;
    }

    /**
     * Enable/disable release events.
     * @param enable Boolean to enable/disable release events.
     */
    void enableReleaseEvent(bool enable)
    {
        releaseEnabled = enable;
    }

    /**
     * Enable/disable counter events.
     * @param enable Boolean to enable/disable counter events.
     */
    void enableCounterEvent(bool enable)
    {
        counterEnabled = enable;
    }

    /**
     * Enable/disable hold events.
     * @param enable Boolean to enable/disable hold events.
     */
    void enableHoldEvent(bool enable)
    {
        holdEnabled = enable;
    }

    // Handles
    /**
     * Interrupt handler for button state changes.
     * Handles debounce, press, release, hold, and counter events.
     */
    inline void handleInterrupt()
    {
        if (!allEnabled)
        {
            return;
        }

        bool enableAll = allEnabled;
        enableAllEvents(false);
        uint32_t currentTime = getTime();
        currentButtonState = isPressed();

        // if button state has changed
        if ((currentButtonState != buttonState) && (currentTime - lastDebounceTime > debounceThreshold))
        {
            // Timer updates
            if (currentButtonState)
            {
                pressStartTime = currentTime;
            }
            else
            {
                timeSincePress = currentTime - pressStartTime;
            }

            // Events when state has changed
            if (eventHandler)
            {
                // Counter
                if ((currentButtonState == !counterReleasedMode) && counterEnabled)
                {
                    if (counter > 0)
                    {
                        counter--;
                    }
                    if (counter == 0)
                    {
                        eventHandler(COUNTER, timeSincePress);
                        resetCounter();
                        counterShouldOverride = counterOverrideMode;
                    }
                }
                // Press
                if (currentButtonState && pressEnabled && !(counterShouldOverride && !counterReleasedMode))
                {
                    eventHandler(PRESS, timeSincePress);
                }
                // Release
                if (!currentButtonState && releaseEnabled && !(counterShouldOverride && counterReleasedMode))
                {
                    eventHandler(RELEASE, timeSincePress);
                }
            }
            lastDebounceTime = currentTime;
            buttonState = currentButtonState;
        }

        // Hold
        if (holdEnabled && currentButtonState)
        {
            // First hold event per hold
            if (!holdDidFirst && (currentTime - pressStartTime > holdThreshold))
            {
                eventHandler(HOLD, 0);
                holdStartTime = currentTime;
                holdDidFirst = true;
            }
            // Repeated hold events
            if (holdRepeatMode && (currentTime - holdStartTime >= holdRepeatDelay))
            {
                eventHandler(HOLD, currentTime - holdStartTime);
            }
        }
        else // On release - reset for next hold event
        {
            holdStartTime = currentTime;
            holdDidFirst = false;
        }
        enableAllEvents(enableAll);
    }

    /**
     * Handle interrupt logic from the loop (optional alternative to attachInterrupt).
     */
    inline void handleFromLoop()
    {
        handleInterrupt();
    }
};

#endif // PUSHBUTTON_HPP