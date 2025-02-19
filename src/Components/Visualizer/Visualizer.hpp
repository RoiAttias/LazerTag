#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <Adafruit_NeoPixel.h> // Include the Adafruit NeoPixel library
#include "Utilities/HyperList.hpp" // Include the HyperList utility
#include "Utilities/MoreMath.hpp" // Include additional math utilities

// Struct to hold animation data and timing
struct Animation {
    void (*animationFunc)(Adafruit_NeoPixel*, uint16_t, uint16_t, float); // Function to perform the animation
    byte layer = 0; // Layer of the animation

    uint16_t startIndex; // Starting index on the strip
    uint16_t length; // Number of LEDs covered

    unsigned long durationMS = 0; // Duration of the animation in milliseconds
    bool repeat = false; // Whether the animation repeats

    unsigned long startTimeMS = 0; // Start time of the animation in milliseconds
    bool paused = false; // Whether the animation is paused
    float currentFactor = 0.0f; // Current factor for animation progress
    unsigned long pauseTimeMS = 0; // Time when the animation was paused

    // Constructor
    Animation(void (*animationFunc)(Adafruit_NeoPixel*, uint16_t, uint16_t, float), byte layer, uint16_t startIndex, uint16_t length, unsigned long durationMS, bool repeat = false)
    : animationFunc(animationFunc) // Initialize the animation function
        , startIndex(startIndex) // Initialize the starting index
        , length(length) // Initialize the length
        , durationMS(durationMS) // Initialize the duration
        , repeat(repeat) { // Initialize the repeat flag
    }

    // Method to run the animation and get the current factor
    float run(unsigned long currentTime) {
        if (durationMS != 0) { // Check if the duration is not zero
            currentFactor = float((currentTime - startTimeMS) % durationMS) / durationMS; // Calculate the current factor
        } else {
            currentFactor = 0.0f; // Set the current factor to zero if duration is zero
        }
        return currentFactor; // Return the current factor
    }

    // Method to check if the animation should run
    bool shouldRun(unsigned long currentTime) const {
        return startTimeMS != 0 && (currentTime - startTimeMS < durationMS || repeat); // Check if the animation should run
    }

    // Method to start the animation
    void start(unsigned long currentTime) {
        startTimeMS = currentTime; // Set the start time to the current time
        paused = false; // Unpause the animation
        currentFactor = 0.0f; // Reset the current factor
    }

    // Method to pause the animation
    void pause(unsigned long currentTime) {
        if (!paused) { // Check if the animation is not already paused
            pauseTimeMS = currentTime; // Record the time when the animation is paused
            paused = true; // Pause the animation
        }
    }

    // Method to resume the animation
    void resume(unsigned long currentTime) {
        if (paused) { // Check if the animation is paused
            startTimeMS += (currentTime - pauseTimeMS); // Adjust the start time to account for the pause duration
            paused = false; // Unpause the animation
        }
    }

    // Method to reset the animation
    void reset() {
        startTimeMS = 0; // Reset the start time
        paused = false; // Unpause the animation
        currentFactor = 0.0f; // Reset the current factor
    }
};

// An example for animation function
void rainbowAnimation(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    const uint16_t factorOffset = uint16_t(0xFFFF * factor); // Calculate the factor offset
    const uint16_t offsetPerFactor = 0xFFFF / length; // Calculate the offset per factor
    uint16_t hue; // Variable to store the hue
    for (uint16_t i = 0; i < length; i++) { // Loop through each LED
        hue = (offsetPerFactor * i) + factorOffset; // Calculate the hue for the current LED
        strip->setPixelColor(startIndex + i, Adafruit_NeoPixel::ColorHSV(hue)); // Set the color of the current LED
    }
}

// Visualizer class to manage the strip and animations
class Visualizer {
private:
    Adafruit_NeoPixel strip; // NeoPixel strip instance
    neoPixelType stripType; // Type of the NeoPixel strip
    unsigned long lastUpdateTime = 0; // Last update time in milliseconds
    unsigned long frameIntervalMS; // Frame interval in milliseconds

public:
    HyperList<Animation> animations; // List of animations
    const byte MAX_LAYERS = 8; // Maximum number of layers

    // Constructor
    Visualizer(uint16_t numPixels, uint8_t pin, unsigned long frameIntervalMS, neoPixelType type = NEO_GRB + NEO_KHZ800)
        : strip(numPixels, pin, type) // Initialize the NeoPixel strip
        , stripType(type) // Initialize the strip type
        , frameIntervalMS(frameIntervalMS) { // Initialize the frame interval
    }

    // Initialize the NeoPixel strip
    void init(uint brightness = 50) {
        strip.begin(); // Initialize the strip
        strip.setBrightness(brightness); // Set the brightness
        strip.show(); // Initialize all pixels to 'off'
    }

    // Set the brightness of the strip
    void setBrightness(uint8_t brightness) {
        strip.setBrightness(brightness); // Set the brightness
    }

    // Add a new animation
    void addAnimation(void (*animationFunc)(Adafruit_NeoPixel*, uint16_t, uint16_t, float), uint16_t startIndex, uint16_t length, unsigned long currentTime, unsigned long durationMS, bool repeat = false) {
        Animation anim = {animationFunc, startIndex, length, currentTime, durationMS, repeat}; // Create a new animation
        animations.addend(anim); // Add the animation to the list
        animations.last().start(currentTime); // Start the animation
    }

    // Remove an animation (by index in the HyperList)
    void removeAnimation(uint16_t index) {
        animations.remove(index); // Remove the animation from the list
    }

    // Update all animations
    void loop() {
        unsigned long currentTime = millis(); // Get the current time
        if (currentTime - lastUpdateTime < frameIntervalMS) return; // Frame rate cap
        lastUpdateTime = currentTime; // Update the last update time

        strip.clear(); // Clear the strip

        for (uint16_t i = 0; i < animations.size(); i++) { // Loop through each animation
            Animation& anim = animations[i]; // Get the current animation
            if (anim.shouldRun(currentTime)) { // Check if the animation should run
                float factor = anim.run(currentTime); // Run the animation and get the current factor
                anim.animationFunc(&strip, anim.startIndex, anim.length, factor); // Call the animation function
            } else {
                anim.stop(); // Stop the animation if it should not run
                removeAnimation(i); // Remove the animation from the list
                i--; // Decrement the index to account for the removed animation
            }
        }

        strip.show(); // Show the updated strip
    }
};

#endif // VISUALIZER_HPP