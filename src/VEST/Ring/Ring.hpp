#ifndef RING_HPP
#define RING_HPP

#include <Arduino.h>
#include "Constants/Constants_Vest.h"
#include "Components/Visualizer/Visualizer.hpp"
#include "Utilities/MoreMath.hpp"

// Animation function for a fire effect
void loadAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    int index = mix(factor, startIndex, length + startIndex); // Calculate the index
    float maxDistance = 3.0f; // Set the maximum distance
    uint32_t hueCycleTime = 3000; // Set the hue cycle time
    for (uint16_t i = 0; i < length; i++) { // Loop through each LED
        uint16_t hue = map(millis()%hueCycleTime, 0, hueCycleTime, 0, 65535); // Calculate the hue
        uint8_t brightness = (uint8_t)(pow(
                                            clamp((maxDistance - distance(i,0, index,0)) / maxDistance, 0.0f, 1.0f)
                                            +clamp((maxDistance - distance(i,0, index+length,0)) / maxDistance, 0.0f, 1.0f)
                                            ,3) * 255); // Calculate the color
        strip->setPixelColor(startIndex + i, strip->ColorHSV(hue, 255, brightness)); // Set the LED color
    }
}

void hitAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    for (uint16_t i = 0; i < length; i++) { // Loop through each LED
        uint16_t hue = 0; // Calculate the hue with HP
        uint8_t brightness = (uint8_t)(clamp(1.0f - 2*(distance(factor,0,0.5f,0)), 0.0f, 1.0f) * 255); // Calculate the color
        strip->setPixelColor(startIndex + i, strip->ColorHSV(hue, 255, brightness)); // Set the LED color
    }
}

// Func: loadAnimationFunc, Layer: 1, Start: 0, Len: 10, Duration: 1000, Loop: true
Animation loadAnimation(loadAnimationFunc, 1, 0, stripLength, 1000, true);
// Func: hitAnimationFunc, Layer: 1, Start: 0, Len: 10, Duration: 100, Loop: false
Animation hitAnimation(hitAnimationFunc, 2, 0, stripLength, 100, false);

namespace Ring {
    Visualizer visualizer(stripPin, stripLength, stripFrameIntervalMS);

    void init() {
        visualizer.init(stripBrightness);
    }

    void loop() {
        visualizer.loop();
    }

    void load() {
        visualizer.clearAnimations();
        visualizer.addAnimation(loadAnimation);
    }
    void hit() {
        visualizer.clearAnimations();
        visualizer.addAnimation(hitAnimation);
    }
}

#endif // RING_HPP