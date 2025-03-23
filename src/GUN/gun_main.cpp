#include <Arduino.h>

#include "Components/IRremoteESP32/IRremoteESP32.hpp"
#include "Components/Visualizer/Visualizer.hpp"
#include "Components/Pushbutton/Pushbutton.hpp"

#include "Utilities/MoreMath.hpp"

#include "Modules/Gun.hpp"
#include "Modules/Player.hpp"

#include "GUI_Gun/GUI_Gun.hpp"
#include "Constants/Constants_Gun.h"

#include "Components/Nexus/Nexus.hpp"
#include "LazerTagPacket.hpp"  // This file remains unchanged

// Forward declaration for the trigger interrupt function.
void gun_trigger_interrupt();

// --- Fire Animation Function ---
// Creates a fire-like effect on an LED strip using random hues and saturation.
void fireAnimationFunc(Adafruit_NeoPixel* strip, uint16_t startIndex, uint16_t length, float factor) {
    for (uint16_t i = 0; i < length; i++) {
        uint16_t hue = random(26, 36) << 8; // Random hue between 26 and 36 (shifted for HSV)
        uint8_t saturation = random(192, 255); // Random saturation between 192 and 255
        uint8_t value = mix(factor < 0.5 ? factor * 2 : 1 - (factor * 2), 0, 255); // Calculate brightness
        strip->fill(Adafruit_NeoPixel::ColorHSV(hue, saturation, value), startIndex, length);
    }
}

// Create an Animation object for the fire effect.
Animation fireAnimation(fireAnimationFunc, 1, 0, 10, 100, false);

// --- Global Objects ---
// Player object manages player data and contains a Gun instance.
Player player(DEVICE_ID);

// Pushbutton for the trigger with an interrupt handler.
Pushbutton trigger(triggerPin, 0, false, gun_trigger_interrupt);

// IR sender and Visualizer for managing IR signals and LED animations.
IRsender irSender(irPin, irChannel, irFrequency);
Visualizer visualizer(stripPin, stripLength, stripFrameIntervalMS);

// A pointer to the Gun inside the player object.
Gun* gun = nullptr;

// IR fire signal.
uint32_t fireSignal = 0XFF00FF00;

// Flag to indicate that the GUI should render.
bool callRender = false;

// --- Interrupt Handler ---
// Called when the trigger pushbutton interrupt occurs.
void gun_trigger_interrupt() {
    trigger.handleInterrupt();
}

// --- Setup Function ---
void setup() {
    Serial.begin(115200);

    // Initialize hardware components.
    trigger.init();
    irSender.init();
    visualizer.init(stripBrightness);
    
    // Initialize Nexus networking with the device address.
    Nexus::begin(NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUPS, NEXUS_DEVICE_ID));
    
    // Link gun pointer to the player's gun.
    gun = &player.gun;
    
    // Initialize the GUI using the player object.
    GUI::init(&player);
    
    // Configure player health.
    player.setHP(60);
    
    // Enable pushbutton press events.
    trigger.enablePressEvent(true);
    
    // Start the gun.
    gun->start();
    callRender = true;
}

// --- Main Loop ---
void loop() {
    // Update Nexus networking.
    Nexus::loop();

    // Update the gun and LED visualizer.
    gun->loop();
    visualizer.loop();
    
    // Update the GUI.
    GUI::loop();
    
    // Auto-reload the gun if out of ammo.
    if (gun->getAmmo() == 0) {
        if (gun->reload()) {
            callRender = true;
        }
    }
    
    // Check the trigger and shoot if pressed.
    if (trigger.hasPressed()) {
        if (gun->shoot()) {
            // On a successful shot, send the IR fire signal and add the fire animation.
            irSender.sendNEC(fireSignal);
            visualizer.addAnimation(fireAnimation);
            callRender = true;
        }
    }
    
    // Process any Nexus packets.
    NexusPacket nexusPacket;
    while (Nexus::readPacket(nexusPacket)) {
        // Process the packet based on its command.
        switch (nexusPacket.command)
        {
            case COMMS_PLAYERHP:
                // Update the player's health.
                int newHP;
                memcpy(&newHP, nexusPacket.payload, sizeof(int));
                player.setHP(newHP);
                break;
            case COMMS_GUNPARAMS:
                // Update the gun's parameters.
                GunData gunData;
                memcpy(&gunData, nexusPacket.payload, sizeof(GunData));
                gun->setData(gunData);
                break;
            case COMMS_FIRECODE:
                // Update the fire signal.
                fireSignal = memcpy(&fireSignal, nexusPacket.payload, sizeof(uint32_t));
                break;
            case COMMS_GAMESTATUS:
                // Update the game status.
                
                break;
            
        }
        callRender = true;
    }
    
    // Render the GUI if flagged.
    if (callRender) {
        GUI::callRender();
        callRender = false;
    }
}