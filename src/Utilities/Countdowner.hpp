/**
 * @file Countdowner.hpp
 * @brief Provides a lightweight scheduler for delayed callbacks using a sorted event list.
 *
 * Uses a HyperList of CountdownEvent structs to schedule functions to be called
 * after a specified delay (in milliseconds). Intended for Arduino loops.
 */

 #ifndef COUNTDOWNER_HPP
 #define COUNTDOWNER_HPP
 
 #include <Arduino.h>
 #include "Utilities/HyperList.hpp"
 
 /**
  * @brief Represents a scheduled callback event.
  *
  * Stores the absolute trigger time (millis) and a callback function pointer
  * with an optional integer parameter.
  */
 struct CountdownEvent {
     uint32_t triggerTime;    ///< Absolute time (ms) when callback should fire.
     void (*callback)(int);   ///< Function pointer to call when event is triggered.
     int parameter;           ///< Optional integer parameter passed to callback.
 };
 
 /**
  * @brief Scheduler that manages delayed callback events.
  *
  * Maintains a list of CountdownEvent items in insertion order (unsorted) and
  * checks each loop iteration to execute and remove expired events.
  * Not real-time; suitable for coarse delays in Arduino applications.
  */
 class Countdowner {
 private:
     HyperList<CountdownEvent> events;  ///< List of pending events.
 
 public:
     /**
      * @brief Constructs an empty scheduler.
      */
     Countdowner() {}
 
     /**
      * @brief Destroys the scheduler, clearing any pending events.
      */
     ~Countdowner() {
         events.clear();
     }
 
     /**
      * @brief Schedules a callback to run after a delay.
      *
      * @param delayMillis Delay in milliseconds from now.
      * @param callback    Function pointer to call when delay elapses.
      * @param parameter   Optional integer passed to callback (default -1).
      */
     void addEvent(uint32_t delayMillis, void (*callback)(int), int parameter = -1) {
         CountdownEvent newEvent = { millis() + delayMillis, callback, parameter };
         events.addend(newEvent);
     }
 
     /**
      * @brief Must be called repeatedly (e.g., in loop()); executes due events.
      *
      * Iterates through the event list, comparing current millis() to each
      * event's triggerTime. If the time has passed, invokes the callback
      * and removes the event.
      */
     void loop() {
         uint32_t now = millis();
         for (int i = 0; i < events.size(); ++i) {
             CountdownEvent ev = events[i];
             if (now >= ev.triggerTime) {
                 ev.callback(ev.parameter);
                 events.remove(i);
                 --i; // adjust index after removal
             }
         }
     }
 };
 
 /**
  * @brief Global Countdowner instance for scheduling events.
  *
  * Defined in Countdowner.cpp to allow a single shared scheduler.
  */
 extern Countdowner* countdowner;
 
 #endif // COUNTDOWNER_HPP