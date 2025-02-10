#ifndef TOUCH_XPT2046_HPP
#define TOUCH_XPT2046_HPP

#include "TFT_eSPI.h"

#include "TFT_eSPI_Elements.h"
#include "Components/LuminaUI/Touch.hpp"
#include "Components/Pushbutton/Pushbutton.hpp"

class Touch_XPT2046 : public Touch {
private:
    bool wasTouched;    // Tracks the previous touch state
    uint16_t pressureThreshold = 150;
    int pointSamples = 3; // Number of samples to average touch point
    int pressureSamples = 5; // Number of samples to average touch pressure

public:
    /**
     * @brief Constructor for the Touch_XPT2046 class.
     * @param screen The associated screen.
     */
    Touch_XPT2046(Screen *screen) : Touch(screen), wasTouched(false) {}

    /**
     * @brief Check if the screen is currently being touched.
     * @param samplesCount The number of samples to average.
     * @return True if the screen is being touched, false otherwise.
     */
    virtual bool isTouched(int pressureThreshold, int samplesCount = 1) {
        if (samplesCount > 1) {
            int validSamples = 0;
            for (int i = 0; i < samplesCount; i++) {
                if (LuminaUI::tft_instance->getTouchRawZ() > pressureThreshold) {
                    validSamples++;
                }
            }
            return validSamples >= samplesCount / 2;
        }
        return LuminaUI::tft_instance->getTouchRawZ() > pressureThreshold;
    }

    /**
     * @brief Get the current touch point, averaging over multiple iterations.
     * @param sampleCount Number of samples to average.
     * @return The average touch point as ivec2.
     */
    virtual ivec2 getPoint(int sampleCount = 1) {
        uint16_t rawTouch[2];
        ivec2 currentTouch(0, 0);
        ivec2 previousTouch(0, 0);
        ivec2 accumulatedTouch(0, 0);  // Accumulates touch coordinates
        ivec2 result(0, 0);
        int validSampleCount = 0;  // Tracks valid touch samples

        const int maxAttempts = sampleCount * 2;  // Allow extra retries for noisy data
        const int noiseThreshold = 50;  // Maximum allowed touch variation

        // Collect multiple samples to average touch coordinates
        // Stop when the required number of valid samples is collected or max attempts are reached
        for (int attempt = 0; attempt < maxAttempts && validSampleCount < sampleCount; ++attempt) {
            LuminaUI::tft_instance->getTouchRaw(&rawTouch[0],&rawTouch[1]);
            currentTouch = ivec2(rawTouch[0], rawTouch[1]);

            // Initialize first sample as reference
            if (validSampleCount == 0) {
                previousTouch = currentTouch;
            }

            // Check if touch point deviation exceeds the noise threshold
            ivec2 delta = currentTouch - previousTouch;
            if (sqrt(pow(delta.x, 2) + pow(delta.y, 2)) > noiseThreshold) {
                continue;  // Ignore noisy sample
            }

            // Accumulate valid touch points
            accumulatedTouch += currentTouch;
            previousTouch = currentTouch;
            validSampleCount++;
        }

        // Return last known point if no valid samples were collected
        if (validSampleCount == 0) {
            return lastPoint;
        }

        // Calculate the averaged touch coordinates
        ivec2 averagedTouch = accumulatedTouch / validSampleCount;

        // Map raw touch coordinates to screen coordinates
        const ivec2 rawMin(140, 245);
        const ivec2 rawMax(4000, 4000);
        const ivec2 screenSize(320, 480);

        ivec2 mappedTouch;
        mappedTouch.x = constrain(map(averagedTouch.x, rawMin.x, rawMax.x, 0, screenSize.x), 0, screenSize.x - 1);
        mappedTouch.y = constrain(screenSize.y - map(averagedTouch.y, rawMin.y, rawMax.y, 0, screenSize.y), 0, screenSize.y);

        // Adjust coordinates based on screen rotation
        switch (LuminaUI::tft_instance->getRotation()) {
            case 1:
                result = ivec2(mappedTouch.y, screenSize.x - 1 - mappedTouch.x);
                break;
            case 2:
                result = ivec2(screenSize.x - 1 - mappedTouch.x, screenSize.y - 1 - mappedTouch.y);
                break;
            case 3:
                result = ivec2(screenSize.y - 1 - mappedTouch.y, mappedTouch.x);
                break;
            default:  // Case 0
                result = mappedTouch;
                break;
        }
        lastPoint = result;
        return result;
    }


    /**
     * @brief Perform touch detection and handle touch events.
     * Put this function in a loop to handle touch events.
     */
    void loop() {
        if (enable) {
            bool isCurrentlyTouched = isTouched(pressureThreshold,pressureSamples);
            if (isCurrentlyTouched && !wasTouched) {
                next(getPoint(pointSamples), true, true); // Press
            } else if (!isCurrentlyTouched && wasTouched) {
                next(lastPoint, true, false); // Release
            } else if (isCurrentlyTouched && wasTouched) {
                next(getPoint(pointSamples), false, true); // Continuous
            }

            if (isCurrentlyTouched || wasTouched) {
                wasTouched = isCurrentlyTouched;
            }
        }
    }
};

#endif // TOUCH_XPT2046_HPP