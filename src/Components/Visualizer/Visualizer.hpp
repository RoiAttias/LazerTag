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

    unsigned long startTimeMS = -1; // Start time of the animation in milliseconds
    bool paused = false; // Whether the animation is paused
    float currentFactor = 0.0f; // Current factor for animation progress
    unsigned long pauseTimeMS = 0; // Time when the animation was paused

    // Constructor
    Animation(void (*animationFunc)(Adafruit_NeoPixel*, uint16_t, uint16_t, float) = nullptr,
    byte layer = 0, uint16_t startIndex = 0, uint16_t length = 0, unsigned long durationMS = 0, bool repeat = false)
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
        return startTimeMS != -1 && ((currentTime - startTimeMS < durationMS) || repeat); // Check if the animation should run
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

    // Method to stop the animation
    void stop() {
        startTimeMS = -1; // Reset the start time
        paused = false; // Unpause the animation
        currentFactor = 0.0f; // Reset the current factor
    }
};

// An example for animation function
void rainbowAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    const uint16_t factorOffset = uint16_t(0xFFFF * factor); // Calculate the factor offset
    const uint16_t offsetPerFactor = 0xFFFF / length; // Calculate the offset per factor
    uint16_t hue; // Variable to store the hue
    for (uint16_t i = 0; i < length; i++) { // Loop through each LED
        hue = (offsetPerFactor * i) + factorOffset; // Calculate the hue for the current LED
        strip->setPixelColor(startIndex + i, Adafruit_NeoPixel::ColorHSV(hue)); // Set the color of the current LED
    }
}

// func: rainbowAnimationFunc, layer: 0, startIndex: 0, length: 10, durationMS: 1000, repeat: true
Animation rainbowAnimation(rainbowAnimationFunc, 0, 0, 10, 1000, true); // Create a rainbow animation

// Visualizer class to manage the strip and animations
class Visualizer {
private:
    const neoPixelType type = NEO_GRB + NEO_KHZ800; // Type of the NeoPixel strip
    const uint8_t gOffset = 0; // Green offset
    const uint8_t rOffset = 1; // Red offset
    const uint8_t bOffset = 2; // Blue offset


    Adafruit_NeoPixel strip; // NeoPixel strip instance
    unsigned long lastUpdateTime = 0; // Last update time in milliseconds
    unsigned long frameIntervalMS; // Frame interval in milliseconds

public:
    HyperList<Animation> animations; // List of animations
    const byte MAX_LAYERS = 8; // Maximum number of layers

    // Constructor
    Visualizer(uint8_t pin, uint16_t numPixels, unsigned long frameIntervalMS)
        : strip(numPixels, pin, type) // Initialize the NeoPixel strip
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
    void addAnimation(Animation anim) {
        anim.start(millis()); // Start the animation
        animations.addend(anim); // Add the animation to the list
    }

    // Construct and add a new animation
    void addAnimation(void (*animationFunc)(Adafruit_NeoPixel*, uint16_t, uint16_t, float),
        uint8_t layer, uint16_t startIndex, uint16_t length, unsigned long durationMS, bool repeat = false) {
        Animation anim(animationFunc, layer, startIndex, length, durationMS, repeat); // Create a new animation
        addAnimation(anim); // Add the animation
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

        for (uint8_t currentLayer = 0; currentLayer < MAX_LAYERS; currentLayer++) { // Loop through each layer
            for (uint16_t i = 0; i < animations.size(); i++) { // Loop through each animation
                if (animations[i].layer == currentLayer) { // Check if the animation is on the current layer
                    if (animations[i].shouldRun(currentTime)) { // Check if the animation should run
                        float factor = animations[i].run(currentTime); // Run the animation and get the current factor
                        animations[i].animationFunc(&strip, animations[i].startIndex, animations[i].length, factor); // Call the animation function
                    } else {
                        removeAnimation(i); // Remove the animation from the list
                        i--; // Decrement the index to account for the removed animation
                    }
                }
            }
        }

        strip.show(); // Show the updated strip
    }
};

#endif // VISUALIZER_HPP