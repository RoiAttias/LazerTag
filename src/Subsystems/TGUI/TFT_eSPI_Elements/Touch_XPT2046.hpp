#ifndef TOUCH_XPT2046_HPP
#define TOUCH_XPT2046_HPP

#include "TFT_eSPI.h"

#include "TFT_eSPI_Elements.hpp"
#include "Subsystems/TGUI/Touch.hpp"
#include "Components/Pushbutton/Pushbutton.hpp"

class Touch_XPT2046 : public Touch {
private:
    bool wasTouched;    // Tracks the previous touch state

public:
    /**
     * Constructor
     * @param screen The associated screen.
     * @param isr Pointer to the Pushbutton instance for handling interrupts.
     */
    Touch_XPT2046(Screen *screen) : Touch(screen), wasTouched(false) {}

    /**
     * Initialize the touch system.
     */
    virtual void init(int enable) override {
        Touch::init(enable);
    }

    /**
     * Check if the screen is being touched.
     * Uses getTouchRawZ() for touch detection.
     */
    virtual bool isTouched() {
        return TGUI::tft_instance->getTouchRawZ() > 200;
    }

    /**
     * Get the current touch point, averaging over multiple iterations.
     * @param iterations Number of samples to average.
     * @return The average touch point as ivec2.
     */
    virtual ivec2 getPoint(int iterations = 1) {
        uint16_t x, y, x_sum = 0, y_sum = 0;
        uint16_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;

        for (int i = 0; i < iterations; ++i) {
            TGUI::tft_instance->getTouchRaw(&x0, &y0);

            if (i == 0) {
                x1 = x0;
                y1 = y0;
            }

            if (sqrt(pow(x0 - x1, 2) + pow(y0 - y1, 2)) > 50) {
                --iterations; // Ignore noisy touch
                continue;
            }

            x_sum += x0;
            y_sum += y0;
            x1 = x0;
            y1 = y0;
        }

        uint16_t xx = x_sum / iterations;
        uint16_t yy = y_sum / iterations;

        // Map raw values to screen coordinates
        xx = constrain(map(xx, 140, 4000, 0, 320), 0, 319);
        yy = constrain(480 - map(yy, 245, 4000, 0, 480), 0, 480);

        // Adjust based on screen rotation
        switch (TGUI::tft_instance->getRotation()) {
            case 1:
                x = yy;
                y = 319 - xx;
                break;
            case 2:
                x = 319 - xx;
                y = 479 - yy;
                break;
            case 3:
                x = 479 - yy;
                y = xx;
                break;
            default: // Case 0
                x = xx;
                y = yy;
                break;
        }

        lastPoint = ivec2(x, y);
        return lastPoint;
    }

    /**
     * Handle touch events in the loop.
     * Detect press, release, and continuous touch.
     */
    void loop() {
        if (enable) {
            bool isCurrentlyTouched = isTouched();
            if (!isCurrentlyTouched && wasTouched) {
                // Touch ended
                next(getPoint(), true, true); // Press
            } else if (!isCurrentlyTouched && wasTouched) {
                // Touch ended
                next(getPoint(), true, false); // Release
            } else if (isCurrentlyTouched && wasTouched) {
                // Continuous touch
                next(getPoint(), false, true); // Continuous
            }

            wasTouched = isCurrentlyTouched;
        }
    }
};

#endif // TOUCH_XPT2046_HPP