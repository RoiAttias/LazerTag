#ifndef SHOOTER_HPP
#define SHOOTER_HPP

#include "Components/IRremoteESP32/IRremoteESP32.hpp"
#include "Components/Visualizer/Visualizer.hpp"

#include "Utilities/MoreMath.hpp"

#include "Modules/gun.hpp"

#include "Constants/Constants_Gun.h"

// Animation function for a fire effect
void fireAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    for (uint16_t i = 0; i < length; i++) { // Loop through each LED
        uint16_t hue = random(26, 36)<<8; // Generate a random hue value in the range [26, 36]
        uint8_t saturation = random(192, 255); // Generate a random saturation value in the range [192, 255]
        uint8_t value = mix(factor, startIndex, length-startIndex-1); // Interpolate the value based on the factor
        strip->setPixelColor(startIndex + i, Adafruit_NeoPixel::ColorHSV(hue, saturation, value)); // Set the color of the current LED
    }
}

// Func: fireAnimationFunc, Layer: 1, Start: 0, Len: 10, Duration: 100, Loop: false
Animation fireAnimation(fireAnimationFunc, 1, 0, 10, 100, false); // Create a fire animation

class Shooter
{
private:
    IRsender irSender;
    Visualizer visualizer;
    Gun *gun;
    uint32_t fireSignal;
    bool callRender;
    
public:
    Shooter()
        : irSender(irPin, irChannel, irFrequency),
          visualizer(stripLength, stripPin, stripFrameIntervalMS),
          gun(nullptr),
          fireSignal(0x00FFA25D),
          callRender(false)
    {
    }
    
    /**
     * @brief Initializes the Shooter subsystem.
     * 
     * This function initializes the IR sender and visualizer components,
     * and sets the gun pointer to the player's gun.
     * 
     * @param playerPtr Pointer to the Player object.
     */
    void init(Player *playerPtr)
    {
        irSender.init();
        visualizer.init(stripBrightness);
        gun = &playerPtr->gun;
    }

    /**
     * @brief Starts the gun and sets the callRender flag to true.
     */
    void start()
    {
        gun->start();
        callRender = true;
    }

    /**
     * @brief Ends the Shooter subsystem.
     * 
     * This function stops the gun and performs any necessary cleanup.
     */
    void end()
    {
        gun->end();
    }

    /**
     * @brief Retrieves the current status of the gun.
     * 
     * @return GunStatus The current status of the gun.
     */
    GunStatus getStatus()
    {
        return gun->getStatus();
    }

    /**
     * @brief Attempts to shoot the gun.
     * 
     * This function attempts to shoot the gun. If successful, it sends an IR signal,
     * adds a fire animation to the visualizer, and sets the callRender flag to true.
     * 
     * @return true if the gun was successfully shot, false otherwise.
     */
    bool shoot()
    {
        if (gun->shoot())
        {
            irSender.sendNEC(fireSignal);
            visualizer.addAnimation(fireAnimation);
            callRender = true;
            return true;
        }
        return false;
    }

    /**
     * @brief Reloads the gun.
     * 
     * This function attempts to reload the gun and sets the callRender flag to true if successful.
     * 
     * @return true if the gun was successfully reloaded, false otherwise.
     */
    bool reload()
    {
        if (gun->reload())
        {
            callRender = true;
            return true;
        }
        return false;
    }

    /**
     * @brief Checks if the visualizer should render.
     * 
     * This function returns the value of the callRender flag,
     * indicating whether the visualizer should render the next frame.
     * 
     * @return true if the visualizer should render, false otherwise.
     */
    bool shouldRender()
    {
        return callRender;
    }

    /**
     * @brief Main loop function for the Shooter subsystem.
     * 
     * This function updates the gun and visualizer components.
     */
    void loop()
    {
        gun->loop();
        visualizer.loop();
    }
};
#endif // SHOOTER_HPP