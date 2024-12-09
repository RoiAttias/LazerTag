#ifndef PUSHBUTTON_HPP
#define PUSHBUTTON_HPP

#include <Arduino.h>
#include <map>

class Pushbutton;
Pushbutton* Pushbutton_Instance;
void Pushbutton_Register(int pbPin, Pushbutton *pbPtr);
void IRAM_ATTR Pushbutton_ISR(void *arg);
// Use std::map for Pushbutton registration
std::map<int, Pushbutton*> registeredPushbuttons;

class Pushbutton
{
public:
    // Enum for event types
    enum EventType
    {
        PRESS,
        RELEASE,
        COUNTER,
    };

    // Define the handler function type
    using EventHandler = void (*)(EventType, uint32_t timeSincePress);

private:
    // Pin configuration and debounce timing
    const uint8_t pin;     // Pin number the button is connected to
    void* pinValue;
    const bool microsMode; // Flag to determine whether to use micros() or millis()
    uint32_t debounceThreshold; // Debounce threshold in milliseconds/microseconds

    // Button state tracking
    volatile bool buttonState;           // Last stable state of the button (pressed/released)
    volatile bool currentButtonState;    // Current state of the button (pressed/released)
    volatile uint32_t lastDebounceTime;  // Timestamp of the last valid state change
    volatile uint32_t pressStartTime;    // Timestamp when the button was last pressed
    volatile uint32_t timeSincePress;    // Time elapsed since the button was last pressed
    volatile bool counterShouldOverride; // Flag to override unnecessary events

    // Event handler and flags
    EventHandler eventHandler;    // Unified event handler
    volatile bool allEnabled;     // Enable/disable all events
    volatile bool pressEnabled;   // Enable/disable press events
    volatile bool releaseEnabled; // Enable/disable release events
    volatile bool counterEnabled; // Enable/disable counter events

    // Counter settings
    volatile int counter;     // Current counter value
    int counterTarget;        // Target value for the counter
    bool counterOverrideMode; // Flag for overriding counter behavior
    bool counterReleasedMode; // Flag for triggering counter on release

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
     * @param useMicros         Use micros() instead of millis() for timing.
     */
    Pushbutton(uint8_t buttonPin, uint32_t debounceMs = 50, bool useMicros = false)
        : pin(buttonPin), pinValue(nullptr), microsMode(useMicros), debounceThreshold(debounceMs),
          buttonState(false), currentButtonState(false), lastDebounceTime(0),
          pressStartTime(0), timeSincePress(0), counterShouldOverride(false),
          eventHandler(nullptr), pressEnabled(false), releaseEnabled(false),
          counterEnabled(false), counter(-1), counterTarget(0),
          counterOverrideMode(false), counterReleasedMode(false) {}

    /**
     * Initialize the button with unified event handler and settings.
     * @param handler Unified event handler function.
     * @param enableAll Boolean to enable/disable all events.
     */
    void init(EventHandler handler = nullptr, bool enableAll = true)
    {
        eventHandler = handler;
        pinMode(pin, INPUT_PULLUP);

        Pushbutton_Register(pin, this); // Register the pushbutton instance
        pinValue = reinterpret_cast<void*>(pin);
        attachInterruptArg(digitalPinToInterrupt(pin), Pushbutton_ISR, pinValue, CHANGE);

        enableAllEvents(enableAll);
    }

    inline void ISR_Enable()
    {
        attachInterruptArg(digitalPinToInterrupt(pin), Pushbutton_ISR, pinValue, CHANGE);
    }

    inline void ISR_Disable()
    {
        detachInterrupt(digitalPinToInterrupt(pin));
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

    // Handles
    /**
     * Interrupt handler for button state changes.
     * Handles debounce, press, release, and counter events.
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
        enableAllEvents(enableAll);
    }
};

void Pushbutton_Register(int pbPin, Pushbutton *pbPtr)
{
    //registeredPushbuttons[pbPin] = pbPtr;
    Pushbutton_Instance = pbPtr;
}

void IRAM_ATTR Pushbutton_ISR(void *arg)
{
    //int pbPin = reinterpret_cast<int>(arg);
    //Pushbutton *pbPtr = registeredPushbuttons[pbPin];
    Pushbutton *pbPtr = Pushbutton_Instance;
    if (pbPtr != nullptr)
    {
        pbPtr->ISR_Disable();
        pbPtr->handleInterrupt();
        pbPtr->ISR_Enable();
    }

}

#endif // PUSHBUTTON_HPP