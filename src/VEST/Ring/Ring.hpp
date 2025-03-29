#ifndef RING_HPP
#define RING_HPP

#include <Arduino.h>
#include "Constants/Constants_Vest.h"
#include "Components/Visualizer/Visualizer.hpp"
#include "Utilities/MoreMath.hpp"

// Animation functions
void load1AnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
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

void load2AnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    int index = mix(factor, startIndex, length + startIndex); // Calculate the index
    float maxDistance = 3.0f; // Set the maximum distance
    for (uint16_t i = 0; i < length; i++) { // Loop through each LED
        uint16_t hue = 65535 / 3 * 2; // Calculate the hue
        uint8_t brightness = (uint8_t)(pow(
                                            clamp((maxDistance - distance(i,0, index,0)) / maxDistance, 0.0f, 1.0f)
                                            +clamp((maxDistance - distance(i,0, index+length,0)) / maxDistance, 0.0f, 1.0f)
                                            ,3) * 255); // Calculate the color
        strip->setPixelColor(startIndex + i, strip->ColorHSV(hue, 255, brightness)); // Set the LED color
    }
}

void hitAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    for (uint16_t i = 0; i < length; i++) { // Loop through each LED
        uint16_t hue = mix(Ring::hp/100.0f, 0, 22000); // Calculate the hue with HP
        uint8_t brightness = (uint8_t)(clamp(1.0f - 2*(distance(factor,0,0.5f,0)), 0.0f, 1.0f) * 255); // Calculate the color
        strip->setPixelColor(startIndex + i, strip->ColorHSV(hue, 255, brightness)); // Set the LED color
    }
}

void hpAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    for (uint16_t i = 0; i < Ring::hp*length/100; i++) { // Loop through each LED
        uint8_t brightness = (uint8_t)(mix((factor < 0.5f) ? (factor/2.0f) : (1.0f-(factor/2.0f)), 180, 255)); // Calculate the color
        strip->setPixelColor(startIndex + i, strip->ColorHSV(40000, 255, brightness)); // Set the LED color
    }
}

void markAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    for (uint16_t i = 0; i < length; i++) { // Loop through each LED
        uint8_t brightness = (uint8_t)(clamp(1.0f - 2*(distance(factor,0,0.5f,0)), 0.0f, 1.0f) * 255); // Calculate the color
        strip->setPixelColor(startIndex + i, strip->ColorHSV(0xFFFF/7, 255, brightness)); // Set the LED color
    }
}

// Func: load1AnimationFunc, Layer: 1, Start: 0, Len: 10, Duration: 2000, Loop: true
Animation load1Animation(load1AnimationFunc, 1, 0, stripLength, 2000, true);

// Func: load2AnimationFunc, Layer: 1, Start: 0, Len: 10, Duration: 1000, Loop: true
Animation load2Animation(load2AnimationFunc, 1, 0, stripLength, 1000, true);

// Func: hitAnimationFunc, Layer: 1, Start: 0, Len: 10, Duration: 100, Loop: false
Animation hitAnimation(hitAnimationFunc, 2, 0, stripLength, 100, false);

// Func: hpAnimationFunc, Layer: 0, Start: 0, Len: 10, Duration: 1000, Loop: true
Animation hpAnimation(hpAnimationFunc, 0, 0, stripLength, 1000, true);

// Func: markAnimationFunc, Layer: 2, Start: 0, Len: 10, Duration: 1000, Loop: false
Animation markAnimation(markAnimationFunc, 2, 0, stripLength, 1000, false);

namespace Ring {
    Visualizer visualizer(stripPin, stripLength, stripFrameIntervalMS);
    int hp = 0;

    void init() {
        visualizer.init(stripBrightness);
    }

    void loop() {
        visualizer.loop();
    }

    void load1() {
        visualizer.clearAnimations();
        visualizer.addAnimation(load1Animation);
    }

    void load2() {
        visualizer.clearAnimations();
        visualizer.addAnimation(load2Animation);
    }

    void hit() {
        visualizer.addAnimation(hitAnimation);
    }

    void onGameStart() {
        visualizer.clearAnimations();
        visualizer.addAnimation(hpAnimation);
    }

    void hpUpdate(int hp) {
        Ring::hp = hp;
        visualizer.addAnimation(hpAnimation);
    }

    void mark() {
        visualizer.addAnimation(markAnimation);
    }
}

#endif // RING_HPP