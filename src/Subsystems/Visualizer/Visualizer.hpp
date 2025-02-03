#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <Adafruit_NeoPixel.h>
#include "HyperList.hpp" // Assuming your custom HyperList library is available

// Struct to hold animation data
struct AnimationSegment {
    uint16_t startIndex;                // Starting index on the strip
    uint16_t length;                    // Number of LEDs covered
    void (*animationFunc)(uint32_t*, uint16_t); // Function to perform the animation
};

// Visualizer class to manage the strip and animations
class Visualizer {
private:
    Adafruit_NeoPixel strip;
    HyperList<AnimationSegment> animations;    // List of active animations

public:
    // Constructor
    Visualizer(uint16_t numPixels, uint8_t pin, neoPixelType type = NEO_GRB + NEO_KHZ800)
        : strip(numPixels, pin, type) {}

    // Initialize the NeoPixel strip
    void begin() {
        strip.begin();
        strip.show(); // Initialize all pixels to 'off'
    }

    // Add a new animation
    void addAnimation(uint16_t startIndex, uint16_t length, void (*animationFunc)(uint32_t*, uint16_t)) {
        AnimationSegment anim = {startIndex, length, animationFunc};
        animations.addend(anim);
    }

    // Remove an animation (by index in the HyperList)
    void removeAnimation(uint16_t index) {
        animations.remove(index);
    }

    // Update all animations
    void update() {
        for (uint16_t i = 0; i < animations.size(); i++) {
            AnimationSegment& anim = animations[i];
            uint32_t tempBuffer[anim.length]; // Temporary buffer for animation
            anim.animationFunc(tempBuffer, anim.length);

            // Update the corresponding section on the strip
            for (uint16_t j = 0; j < anim.length; j++) {
                strip.setPixelColor(anim.startIndex + j, tempBuffer[j]);
            }
        }
        strip.show(); // Apply all changes
    }
};

#endif // VISUALIZER_HPP