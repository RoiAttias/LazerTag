#ifndef COUNTDOWNER_HPP
#define COUNTDOWNER_HPP

#include <Arduino.h>
#include "Utilities/HyperList.hpp"

// Define a type for our callback function.
typedef void (*Callback)(int);

// Struct that holds the trigger time (in millis) and a callback pointer.
struct CountdownEvent {
  uint32_t triggerTime;  // Absolute time (in millis) when the callback should be executed.
  Callback callback;
  int parameter; // Optional parameter for the callback function.
};

// Countdowner class that manages a sorted linked list (HyperList) of CountdownEvents.
class Countdowner {
private:
    HyperList<CountdownEvent> events; // List of countdown events.
  
public:
  Countdowner() {}

  // Destructor cleans up any remaining nodes.
  ~Countdowner() {
    events.clear();
  }

  // Add an event that will trigger after delayMillis milliseconds from now.
  // The event is inserted in sorted order (by triggerTime) in the linked list.
  void addEvent(uint32_t delayMillis, Callback callback, int parameter = -1) {
    CountdownEvent newEvent = {millis() + delayMillis, callback, parameter};
    events.addend(newEvent);
  }

  // Call this function in your main loop.
  // It checks the list for any events whose triggerTime is now (or passed) and executes their callbacks.
  void loop() {
    uint32_t now = millis();
    CountdownEvent event;
    for (size_t i = 0; i < events.size(); i++) {
        event = events[i];
        if (event.triggerTime >= now) {
            event.callback(event.parameter); // Call the callback function with the parameter.
            events.remove(i); // Remove the event from the list.
            i--; // Adjust index after removal.
        }
    }
  }
};

Countdowner * countdowner = new Countdowner; // Global instance of Countdowner.

#endif // COUNTDOWNER_HPP