#ifndef VISUALIZER_HPP
#define VISUALIZER_HPP

#include <Adafruit_NeoPixel.h>
#include "Utilities/HyperList.hpp"
#include "Utilities/MoreMath.hpp"

// Struct to hold animation data
struct AnimationSegment {
    uint16_t startIndex;                // Starting index on the strip
    uint16_t length;                    // Number of LEDs covered
    void (*animationFunc)(uint32_t*, uint16_t, float); // Function to perform the animation
};

// Struct to hold animation timing data
struct AnimationTiming {
    AnimationSegment anim;
    unsigned long startTimeMS = 0;        // Start time of the animation
    unsigned long durationMS = 0;      // Duration of the animation in milliseconds
    bool repeat = false;           // Whether the animation repeats
    bool paused = false;           // Whether the animation is paused
    float currentFactor = 0.0f;    // Current factor for animation progress

    float run(unsigned long currentTime) {
        currentFactor = float(currentTime - startTimeMS) / durationMS;
        return currentFactor;
    }

    bool isRunning() const {
        return startTimeMS != 0 && !paused && currentFactor <= 1.0f;
    }

    bool shouldRun(unsigned long currentTime) const {
        return isRunning() && (currentTime - startTimeMS < durationMS || repeat);
    }

    void start(unsigned long currentTime) {
        startTimeMS = currentTime;
        paused = false;
        currentFactor = 0.0f;
    }

    void pause() {
        paused = true;
    }

    void resume(unsigned long currentTime) {
        if (paused) {
            startTimeMS = currentTime - (unsigned long)(currentFactor * durationMS);
            paused = false;
        }
    }

    void stop() {
        startTimeMS = 0;
        paused = false;
        currentFactor = 0.0f;
    }
};

// An example for animation function
void rainbowAnimation(uint32_t* buffer, uint16_t length, float factor = 0.0f) {
    const uint16_t factorOffset = uint16_t(0xFFFF * factor);
    const uint16_t offsetPerFactor = 0xFFFF / length;
    uint16_t hue;
    for (uint16_t i = 0; i < length; i++) {
        hue = (offsetPerFactor * i) + factorOffset;
        buffer[i] = Adafruit_NeoPixel::ColorHSV(hue);
    }
}

// Visualizer class to manage the strip and animations
class Visualizer {
private:
    Adafruit_NeoPixel strip;
    neoPixelType stripType;
    unsigned long lastUpdateTime = 0;
    unsigned long frameIntervalMS;

public:
    HyperList<AnimationTiming> animations;
    // Constructor
    Visualizer(uint16_t numPixels, uint8_t pin, unsigned long frameIntervalMS, neoPixelType type = NEO_GRB + NEO_KHZ800)
        : strip(numPixels, pin, type), stripType(type) {
        calculateOffsets();
    }

    // Initialize the NeoPixel strip
    void init(uint brightness = 50) {
        strip.begin();
        strip.setBrightness(brightness);
        strip.show(); // Initialize all pixels to 'off'
    }

    // Function to calculate the offsets based on neoPixelType
    void calculateOffsets() {
        wOffset = (stripType >> 6) & 0b11;
        rOffset = (stripType >> 4) & 0b11;
        gOffset = (stripType >> 2) & 0b11;
        bOffset = stripType & 0b11;
    }

    // Set the brightness of the strip
    void setBrightness(uint8_t brightness) {
        strip.setBrightness(brightness);
    }

    // Add a new animation
    void addAnimation(uint16_t startIndex, uint16_t length, void (*animationFunc)(uint32_t*, uint16_t, float), unsigned long durationMS, bool repeat = false) {
        AnimationTiming animTiming = {{startIndex, length, animationFunc}, millis(), durationMS, repeat};
        animations.addend(animTiming);
    }

    // Remove an animation (by index in the HyperList)
    void removeAnimation(uint16_t index) {
        animations.remove(index);
    }

    // Update all animations
    void loop() {
        unsigned long currentTime = millis();
        if (currentTime - lastUpdateTime < frameIntervalMS) return; // Frame rate cap
        lastUpdateTime = currentTime;

        strip.clear();
        for (uint16_t i = 0; i < animations.size(); i++) {
            AnimationTiming& animTiming = animations[i];
            if (animTiming.shouldRun(currentTime)) {
                float factor = animTiming.run();
                animTiming.anim.animationFunc(strip.getPixels(), animTiming.anim.length, factor);
                strip.show();
            } else {
                animTiming.stop();
            }
        }

        strip.show();
    }
};

#endif // VISUALIZER_HPP