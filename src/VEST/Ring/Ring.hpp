#ifndef RING_HPP
#define RING_HPP

#include <Arduino.h>
#include "Constants/Constants_Vest.h"
#include "Components/Visualizer/Visualizer.hpp"
#include "Utilities/MoreMath.hpp"

#define HUE_RED 0
#define HUE_GREEN 0xFFFF / 3
#define HUE_BLUE HUE_GREEN * 2
#define HUE_YELLOW HUE_GREEN / 2

namespace Ring {
    extern int hp;
    extern uint16_t flashingHue;
}

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
        uint16_t hue = HUE_BLUE; // Set the hue to blue
        uint8_t brightness = (uint8_t)(pow(
                                            clamp((maxDistance - distance(i,0, index,0)) / maxDistance, 0.0f, 1.0f)
                                            +clamp((maxDistance - distance(i,0, index+length,0)) / maxDistance, 0.0f, 1.0f)
                                            ,3) * 255); // Calculate the color
        strip->setPixelColor(startIndex + i, strip->ColorHSV(hue, 255, brightness)); // Set the LED color
    }
}

void hitAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    for (uint16_t i = 0; i < length; i++) { // Loop through each LED
        uint16_t hue = mix(Ring::hp/100.0f, HUE_RED, HUE_GREEN); // Calculate the hue with HP
        uint8_t brightness = (uint8_t)(clamp(1.0f - 2*(distance(factor,0,0.5f,0)), 0.0f, 1.0f) * 255); // Calculate the color
        strip->setPixelColor(startIndex + i, strip->ColorHSV(hue, 255, brightness)); // Set the LED color
    }
}

void hpAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    for (uint16_t i = 0; i < length*Ring::hp/100; i++) { // Loop through each LED
        uint8_t brightness = (uint8_t)(mix((factor < 0.5f) ? (factor/2.0f) : (1.0f-(factor/2.0f)), 180, 255)); // Calculate the color
        strip->setPixelColor(startIndex + i, strip->ColorHSV(40000, 255, brightness)); // Set the LED color
    }
}

void markedAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    for (uint16_t i = 0; i < length; i++) { // Loop through each LED
        uint8_t brightness = (uint8_t)(clamp(1.0f - 2*(distance(factor,0,0.5f,0)), 0.0f, 1.0f) * 255); // Calculate the color
        strip->setPixelColor(startIndex + i, strip->ColorHSV(Ring::flashingHue, 255, brightness)); // Set the LED color
    }
}

void winAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    int current = (int)(factor * 6);
    if (current >= 0 && current < 3) {
        for (uint16_t i = 0; i < length; i++) { // Loop through each LED
            strip->setPixelColor(startIndex + i, strip->ColorHSV(HUE_GREEN, 255, 255 * (i % 3 == current)));
        }
    } else if (current == 3) {
        for (uint16_t i = 0; i < length*(factor*6 - 3); i++) { // Loop through each LED
            strip->setPixelColor(startIndex + i, strip->ColorHSV(HUE_GREEN, 255, 255));
        }
    } else if (current == 4) {
        for (uint16_t i = length; i > length*(1-(factor*6 - 4)); i--) { // Loop through each LED
            strip->setPixelColor(startIndex + i, strip->Color(0,0,0));
        }
    }
    else if (current == 5 || current == 6) {
        for (uint16_t i = 0; i < length; i++) { // Loop through each LED
            float calcFact = (factor*6 - 5) / 2;
            uint8_t brightness = (uint8_t)(clamp(1.0f - 2*(distance(calcFact,0,0.5f,0)), 0.0f, 1.0f) * 255); // Calculate the color
            strip->setPixelColor(startIndex + i, strip->ColorHSV(HUE_GREEN, 255, brightness)); // Set the LED color
        }
    }
}

void loseAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    int current = (int)(factor * 6);
    if (current == 0 || current == 1) {
        int point1 = length / 4;
        int point2 = length * 3 / 4;
        float dist1, dist2, minDist, conDist;
        for (uint16_t i = 0; i < length; i++) { // Loop through each LED
            dist1 = distance(i,0, point1,0);
            dist2 = distance(i,0, point2,0);
            minDist = min(dist1, dist2);
            conDist = current == 0 ? mix(factor * 6, 0, length/2) : mix(factor * 6 - 1, length/2, 0);
            strip->setPixelColor(startIndex + i, strip->ColorHSV(HUE_RED, 255, 255 * (minDist < conDist)));
        }
    } else if (current == 3) {
        for (uint16_t i = 0; i < length*(factor*6 - 3); i++) { // Loop through each LED
            strip->setPixelColor(startIndex + i, strip->ColorHSV(HUE_RED, 255, 255));
        }
    } else if (current == 4) {
        for (uint16_t i = length; i > length*(1-(factor*6 - 4)); i--) { // Loop through each LED
            strip->setPixelColor(startIndex + i, strip->Color(0,0,0));
        }
    }
    else if (current == 5 || current == 6) {
        for (uint16_t i = 0; i < length; i++) { // Loop through each LED
            float calcFact = (factor*6 - 5);
            uint8_t brightness = (uint8_t)(clamp(1.0f - 2*(distance(calcFact,0,0.5f,0)), 0.0f, 1.0f) * 255); // Calculate the color
            strip->setPixelColor(startIndex + i, strip->ColorHSV(HUE_RED, 255, brightness)); // Set the LED color
        }
    }
}

// Func: load1AnimationFunc, Layer: 1, Start: 0, Len: 24, Duration: 2000, Loop: true
Animation load1Animation(load1AnimationFunc, 1, 0, stripLength, 2000, true);

// Func: load2AnimationFunc, Layer: 1, Start: 0, Len: 24, Duration: 500, Loop: true
Animation load2Animation(load2AnimationFunc, 1, 0, stripLength, 500, true);

// Func: hitAnimationFunc, Layer: 1, Start: 0, Len: 24, Duration: 100, Loop: false
Animation hitAnimation(hitAnimationFunc, 2, 0, stripLength, 100, false);

// Func: hpAnimationFunc, Layer: 0, Start: 0, Len: 24, Duration: 1000, Loop: true
Animation hpAnimation(hpAnimationFunc, 0, 0, stripLength, 1000, true);

// Func: markedAnimationFunc, Layer: 2, Start: 0, Len: 24, Duration: 1000, Loop: false
Animation markedAnimation(markedAnimationFunc, 2, 0, stripLength, 1000, false);

// Func: countdownAnimationfunc, Layer: 2, Start: 0, Len: 6, Duration: 600, Loop: false
Animation countdownAnimation(markedAnimationFunc, 2, 0, stripLength / 4, 600, false);

// Func: winAnimationFunc, Layer: 1, Start: 0, Len: 24, Duration: 5000, Loop: true
Animation winAnimation(winAnimationFunc, 1, 0, stripLength, 5000, true);

// Func: loseAnimationFunc, Layer: 1, Start: 0, Len: 24, Duration: 5000, Loop: true
Animation loseAnimation(loseAnimationFunc, 1, 0, stripLength, 5000, true);

namespace Ring {
    Visualizer visualizer(stripPin, stripLength, stripFrameIntervalMS);
    int hp = 0;
    uint16_t flashingHue;

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

    void countdown(int num) {
        if (num > 3) {
            return;
        } else if (num == 0) {
            flashingHue = HUE_GREEN;
            num = 3;
        } else {
            flashingHue = HUE_RED;
        }
        visualizer.clearAnimations();
        
        for (int i = 0; i < num; i++) {
            countdownAnimation.startIndex = i * stripLength / 3;
            visualizer.addAnimation(countdownAnimation);
        }
    }

    void hit(int hp) {
        Ring::hp = hp;
        visualizer.addAnimation(hitAnimation);
    }

    void onGameStart(int hp) {
        visualizer.clearAnimations();
        visualizer.addAnimation(hpAnimation);
    }

    void mark() {
        flashingHue = HUE_YELLOW;
        visualizer.addAnimation(markedAnimation);
    }

    void win() {
        visualizer.clearAnimations();
        visualizer.addAnimation(winAnimation);
    }

    void lose() {
        visualizer.clearAnimations();
        visualizer.addAnimation(loseAnimation);
    }
}

#endif // RING_HPP