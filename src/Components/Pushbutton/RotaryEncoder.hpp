#ifndef ROTARYENCODER_HPP
#define ROTARYENCODER_HPP

#include <Pushbutton.hpp>

class RotaryEncoder
{
public:
    // Enum for event types
    enum EventType
    {
        UP,
        DOWN
    };

    // Define the handler function type
    using EventHandler = void (*)(EventType, uint32_t timeSinceLast);

private:
    // Pin configuration and debounce/hold timings
    const uint8_t ;
    const uint8_t pin;
    const uint8_t pin;

    const bool microsMode; // Flag to determine whether to use micros() or millis()

    uint32_t debounceThreshold; // Debounce threshold in milliseconds/microseconds
    uint32_t holdThreshold;     // Hold threshold in milliseconds/microseconds

    // Rotary state tracking
    volatile bool buttonState;           // Last stable state of the button (pressed/released)
    volatile bool currentButtonState;    // Current state of the button (pressed/released)
    volatile uint32_t lastDebounceTime;  // Timestamp of the last valid state change
    volatile uint32_t holdStartTime;     // Timestamp when the button hold began
    volatile uint32_t pressStartTime;    // Timestamp when the button was last pressed
    volatile uint32_t timeSinceLast;    // Time elapsed since the button was last pressed
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
          timeSinceLast(0), counterShouldOverride(false), holdDidFirst(false), eventHandler(nullptr),
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
};

#endif // ROTARYENCODER_HPP