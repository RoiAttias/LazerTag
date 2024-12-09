/*
#include <Arduino.h>
#include "Components/IRremoteESP32/IRsender.hpp"
#include "Components/Pushbutton/Pushbutton.hpp"

// Define pin configurations
const uint8_t buttonPin = 2; // Replace with your button pin
const uint8_t irPin = 4;     // Replace with your IR LED pin

// Define the IR signal
const unsigned long fireSignal = 0xFF00FF; // Example NEC signal for "fire"

volatile bool shotNow = false;

// Instances of classes
Pushbutton trigger(buttonPin);
IRsender irSender(irPin, 0, 38000); // Using channel 0 and 38kHz frequency

// Event handler for button
void handleButtonEvent(Pushbutton::EventType eventType, uint32_t timeSincePress) {
    if (eventType == Pushbutton::PRESS) {
        shotNow = true; // Flag to indicate the shot should be fired
    }
}

void setup() {
    Serial.begin(115200);

    // Initialize the trigger button
    trigger.init(handleButtonEvent, true); // Enable all events

    // Additional setup logic if needed
    Serial.println("Laser Tag Gun Ready!");
}

void loop() {
    if (shotNow)
    {
        irSender.sendNEC(fireSignal); // Send fire signal when the trigger is pressed
        shotNow = false; // Reset the shot flag
    }

    // The rest of the program primarily relies on interrupts, so no logic is needed here
    delay(10); // Optional: Small delay to stabilize processing
}
*/