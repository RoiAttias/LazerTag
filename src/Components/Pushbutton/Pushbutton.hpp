/**
 * @file Pushbutton.hpp
 * @brief Debounced pushbutton handler with optional ISR and event flags.
 *
 * Provides edge-detection, debounce filtering (ms or μs), and event queries
 * for press and release using a HyperList-like approach (flags).
 */

 #ifndef PUSHBUTTON_HPP
 #define PUSHBUTTON_HPP
 
 #include <Arduino.h>
 
 /**
  * @brief Button event types.
  */
 enum PushbuttonStatus : byte {
     PRESS,             ///< Button press event
     RELEASE,           ///< Button release event
     PushbuttonStatus_size
 };
 
 /**
  * @brief Callback type for ISR handling button state changes.
  */
 using ISRpointer = void (*)();
 
 /**
  * @class Pushbutton
  * @brief Debounces a GPIO button and tracks press/release events.
  *
  * Supports optional micros()-based timing, attachInterrupt, and
  * querying press/release events via hasPressed/hasReleased.
  */
 class Pushbutton {
 private:
     uint8_t pin;                      ///< GPIO pin number
     uint32_t debounceThreshold;       ///< Debounce time threshold
     bool useMicros;                   ///< True to use micros(), false for millis()
     ISRpointer isrPointer;            ///< ISR callback for state changes
 
     volatile uint32_t lastDebounceTime;   ///< Timestamp of last state change
     volatile bool buttonState;            ///< Logical current state (pressed/unpressed)
     volatile bool lastButtonState;        ///< Previous state for edge detection
 
     volatile uint32_t currentTime;        ///< Timestamp in ISR
     volatile PushbuttonStatus currentState;///< Last recognized event state
 
     volatile bool _hasPressed;            ///< Flag: a press event occurred
     volatile bool _hasReleased;           ///< Flag: a release event occurred
 
     bool enablePress;                     ///< Enable generating press events
     bool enableRelease;                   ///< Enable generating release events
 
 public:
     /**
      * @brief Construct a debounced button handler.
      *
      * @param buttonPin    GPIO pin connected to the button (INPUT_PULLUP).
      * @param debounceMs   Debounce interval (ms or μs if useMicros=true).
      * @param useMicros    True to use micros() for timing, false for millis().
      * @param isr          Optional ISR function for attachInterrupt.
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
 
     /**
      * @brief Destructor detaches the interrupt.
      */
     ~Pushbutton() {
         detachInterrupt(digitalPinToInterrupt(pin));
     }
 
     /**
      * @brief Initialize pin mode and optional interrupt.
      *
      * @param enablePress   True to enable press events.
      * @param enableRelease True to enable release events.
      */
     void init(bool enablePress = false, bool enableRelease = false) {
         pinMode(pin, INPUT_PULLUP);
         if (isrPointer) {
             attachInterrupt(digitalPinToInterrupt(pin), isrPointer, CHANGE);
         }
         enablePressEvent(enablePress);
         enableReleaseEvent(enableRelease);
     }
 
     /**
      * @brief Enable or disable press event generation.
      * @param enable True to enable, false to disable.
      */
     void enablePressEvent(bool enable) {
         enablePress = enable;
     }
 
     /**
      * @brief Enable or disable release event generation.
      * @param enable True to enable, false to disable.
      */
     void enableReleaseEvent(bool enable) {
         enableRelease = enable;
     }

     /**
      * @brief Clear the press and release event flags.
      */
     void clear() {
         _hasPressed = false;
         _hasReleased = false;
     }
 
     /**
      * @brief Query and clear the press event flag.
      * @return True if a press event occurred since last call.
      */
     bool hasPressed() {
         bool state = _hasPressed;
         _hasPressed = false;
         return state;
     }
 
     /**
      * @brief Query and clear the release event flag.
      * @return True if a release event occurred since last call.
      */
     bool hasReleased() {
         bool state = _hasReleased;
         _hasReleased = false;
         return state;
     }
 
     /**
      * @brief Read the current raw button state (active LOW).
      * @return True if button is pressed (LOW), false otherwise.
      */
     bool isPressed() const {
         return digitalRead(pin) == LOW;
     }
 
     /**
      * @brief Get current time based on configuration.
      * @return micros() if useMicros, else millis().
      */
     uint32_t getTime() const {
         return useMicros ? micros() : millis();
     }
 
     /**
      * @brief ISR routine to handle state changes and debounce.
      *
      * Must be fast; it updates press/release flags when thresholds are met.
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