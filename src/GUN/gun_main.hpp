/**
 * @file gun_main.hpp
 * @brief Main firmware logic for the Gun device in the LaserTag system.
 *
 * - Initializes and manages the trigger button, IR sender (NEC protocol), LED visualizer,
 *   and networking via Nexus (ESP-NOW).
 * - Integrates Game, Gun, and Player modules to handle shooting, reloading, and receiving
 *   commands from the Manager.
 * - Delegates on-screen updates to the GUI_Gun subsystem.
 */

#include <Arduino.h>

#include "Components/IRremoteESP32/IRremoteESP32.hpp" ///< IRsender for NEC transmissions
#include "Components/Visualizer/Visualizer.hpp"       ///< LED animation engine
#include "Components/Pushbutton/Pushbutton.hpp"       ///< Debounced button with ISR
#include "Components/Nexus/Nexus.hpp"                 ///< ESP-NOW networking

#include "Utilities/MoreMath.hpp"                     ///< clamp(), distance(), mix()

#include "Constants_Gun.h"                            ///< Pin assignments & timings
#include "Common/LazerTagPacket.hpp"                  ///< COMMS_* command codes

#include "Modules/Game.hpp"                           ///< Shared GameStatus enum
#include "Modules/Gun.hpp"                            ///< Gun logic & data
#include "Modules/Player.hpp"                         ///< Player data model

#include "GUI_Gun/GUI_Gun.hpp"                        ///< On-screen display for gun status

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------

/// ISR handler for the trigger pushbutton
void IRAM_ATTR gun_trigger_interrupt();

/// Callback invoked by Gun when a shot is fired
void gun_Shoot_callback(int parameter);

/// Callback invoked by Gun when reload completes
void gun_OnReloadFinish_callback(Gun* gun);

//-----------------------------------------------------------------------------
// Animations
//-----------------------------------------------------------------------------

bool isDemarked = false; ///< Flag to indicate if the gun is demarked instead of marked

/**
 * @brief Fire animation for the LED strip.
 *
 * Simulates a brief muzzle flash using randomized warm hues and an envelope
 * over the 'factor' (0→1).
 *
 * @param strip       Pointer to the NeoPixel strip instance.
 * @param startIndex  Starting LED index for this effect.
 * @param length      Number of LEDs to affect.
 * @param factor      Progress of the animation (0.0 to 1.0).
 */
void fireAnimationFunc(Adafruit_NeoPixel* strip,
                       uint16_t startIndex,
                       uint16_t length,
                       float factor) {
    for (uint16_t i = 0; i < length; i++) {
        // Warm hue range [26,36] << 8 for HSV
        uint16_t hue = random(26, 36) << 8;
        // High saturation for vividness
        uint8_t saturation = random(192, 255);
        // Brightness envelope: ramp up then down
        float envelope = (factor < 0.5f) ? (factor * 2.0f)
                                         : (1.0f - (factor * 2.0f));
        uint8_t value = mix(envelope, 0, 255);
        // Fill the segment with this HSV color
        strip->fill(
            Adafruit_NeoPixel::ColorHSV(hue, saturation, value),
            startIndex, length
        );
    }
}

/**
 * @brief Mark animation for the LED strip.
 *
 * Flashes a distinct hue (≈yellow) with brightness modulated by 'factor'.
 *
 * @param strip       Pointer to the NeoPixel strip instance.
 * @param startIndex  Starting LED index.
 * @param length      Number of LEDs.
 * @param factor      Progress of animation.
 */
void markAnimationFunc(Adafruit_NeoPixel* strip,
                       uint16_t startIndex,
                       uint16_t length,
                       float factor) {
    for (uint16_t i = 0; i < length; i++) {
        // Brightness falls off symmetrically around midpoint
        float dist = fabs(distance(factor, 0, 0.5f, 0));
        uint8_t brightness = uint8_t(clamp(1.0f - 2.0f * dist,
                                           0.0f, 1.0f) * 255);
        // Fixed hue: 1/7th of the color wheel (yellow-ish)
        uint16_t hue = (0xFFFF / 12); // 360/12 = 30 degrees
        if (isDemarked) {
            hue *= 9; // 270 degrees (Purple)
        } else {
            hue *= 2; // 60 degrees (Yellow)
        }
        strip->setPixelColor(
            startIndex + i,
            strip->ColorHSV(hue, 255, brightness)
        );
    }
}

//-----------------------------------------------------------------------------
// Animation objects
//-----------------------------------------------------------------------------
/// Displayed on each successful shot
Animation fireAnimation(
    fireAnimationFunc,  ///< callback
    1,                  ///< layer (over other effects)
    0,                  ///< start index
    10,                 ///< number of LEDs
    100,                ///< duration (ms)
    false               ///< no loop
);

/// Displayed on receiving a "mark" command
Animation markAnimation(
    markAnimationFunc,
    2,                  ///< higher layer
    0,
    stripLength,        ///< full strip length
    1000,               ///< duration (ms)
    false
);

//-----------------------------------------------------------------------------
// Global device objects
//-----------------------------------------------------------------------------

/// Player data model (ID from SELECT_DEVICE)
Player player(DEVICE_ID);

/// Debounced hardware trigger button (ISR-based)
Pushbutton trigger(
    triggerPin,             ///< GPIO
    0,                       ///< debounce (ISR-managed)
    false,                   ///< useMicros?
    gun_trigger_interrupt    ///< ISR function
);

/// IR NEC transmitter
IRsender irSender(
    irPin,       ///< data pin
    irChannel,   ///< PWM channel
    irFrequency  ///< carrier frequency
);

/// LED strip animator
Visualizer visualizer(
    stripPin,
    stripLength,
    stripFrameIntervalMS
);

/// Gun instance wrapping GunData & logic
Gun gun(
    Sidearm,                   ///< initial parameters
    gun_Shoot_callback,        ///< on shot
    gun_OnReloadFinish_callback///< on reload complete
);

/// Current IR code to transmit when shooting
uint32_t fireSignal = 0x0000000;

/// Flag to request GUI update
bool callRender = false;

/// Local copy of game phase from Manager
GameStatus gameStatus = GameStatus::GAME_WAITING;

//-----------------------------------------------------------------------------
// ISR and callback implementations
//-----------------------------------------------------------------------------

/**
 * @brief ISR for the trigger button.
 *
 * Simply delegates to Pushbutton's handleInterrupt(). Placed in IRAM.
 */
void IRAM_ATTR gun_trigger_interrupt() {
    trigger.handleInterrupt();
}

/**
 * @brief Callback invoked by Gun when a shot fires.
 *
 * Sends IR code, plays fire animation, decrements ammo, and flags GUI redraw.
 *
 * @param parameter  Burst index (unused here).
 */
void gun_Shoot_callback(int /*parameter*/) {
    if (gun.getAmmo() > 0) {
        irSender.sendNEC(fireSignal);
        visualizer.addAnimation(fireAnimation);
        gun.decreaseAmmo();
        callRender = true;
    }
}

/**
 * @brief Callback invoked by Gun when reload finishes.
 *
 * Triggers a full GUI redraw via the GUI subsystem.
 *
 * @param gunPtr  Pointer to the Gun (unused here).
 */
void gun_OnReloadFinish_callback(Gun* /*gunPtr*/) {
    GUI::callRender();
}

//-----------------------------------------------------------------------------
// Arduino setup()
//-----------------------------------------------------------------------------

/**
 * @brief Setup function called once on reset.
 *
 * - Configures trigger, IR sender, and LED strip.
 * - Connects to Nexus network.
 * - Initializes the on-screen GUI.
 * - Prepares gun for firing.
 */
void gun_setup() {
    trigger.init();
    irSender.init();
    visualizer.init(stripBrightness);

    Nexus::begin(NexusAddress(NEXUS_PROJECT_ID,
                              NEXUS_GROUPS,
                              NEXUS_DEVICE_ID));

    GUI::init(&player, &gun);
    GUI::message("Waiting...");

    trigger.enablePressEvent(true);

    gun.start();
    callRender = true;
}

//-----------------------------------------------------------------------------
// Arduino loop()
//-----------------------------------------------------------------------------

/**
 * @brief Main loop, runs continuously.
 *
 * - Advances scheduled callbacks (burst shots).
 * - Processes network packets.
 * - Updates gun logic (reload timers).
 * - Updates LED animations.
 * - Updates GUI state.
 * - Handles auto-reload and manual shooting when game is running.
 * - Renders GUI if flagged.
 */
void gun_loop() {
    // Handle scheduled countdowner events (e.g. burst fire)
    countdowner->loop();

    // ESP-NOW networking
    Nexus::loop();

    // Gun state machine (reloading/shooting)
    gun.loop();

    // LED animations
    visualizer.loop();

    // GUI logic
    GUI::loop();

    // If game is active, manage reload and trigger input
    if (gameStatus == GAME_RUNNING) {
        // Auto-reload
        if (gun.getAmmo() == 0 && gun.reload()) {
            callRender = true;
        }
        // Manual fire
        if (trigger.hasPressed() || (trigger.isPressed() && gun.fullAuto)) {
            gun.shoot();
        }
    }

    // Process incoming Nexus packets
    NexusPacket packet;
    int newHP;
    GunData newParams;
    GameStatus prevStatus;
    while (Nexus::readPacket(packet)) {
        switch (packet.command) {
            case COMMS_PLAYERHP:
                memcpy(&newHP,
                       packet.payload,
                       payloadSizePerCommand[COMMS_PLAYERHP]);
                player.setHP(newHP);
                break;

            case COMMS_GUNPARAMS:
                memcpy(&newParams,
                       packet.payload,
                       payloadSizePerCommand[COMMS_GUNPARAMS]);
                gun.setData(newParams);
                break;

            case COMMS_FIRECODE:
                memcpy(&fireSignal,
                       packet.payload,
                       payloadSizePerCommand[COMMS_FIRECODE]);
                break;

            case COMMS_GAMESTATUS:
                prevStatus = gameStatus;
                memcpy(&gameStatus,
                       packet.payload,
                       payloadSizePerCommand[COMMS_GAMESTATUS]);
                if (prevStatus != gameStatus) {
                    // Update on-screen message & trigger actions
                    switch (gameStatus) {
                        case GAME_WAITING:
                            GUI::message("Waiting...");
                            break;
                        case GAME_STARTING:
                            GUI::message("Starting...");
                            break;
                        case GAME_THREE:
                            GUI::message("3");
                            break;
                        case GAME_TWO:
                            GUI::message("2");
                            break;
                        case GAME_ONE:
                            GUI::message("1");
                            break;
                        case GAME_GO:
                            GUI::message("GO!");
                            gun.reload();
                            break;
                        case GAME_RUNNING:
                            GUI::onGame();
                            break;
                        case GAME_OVER:
                            GUI::message("Game Over!");
                            break;
                        case GAME_WON:
                            GUI::message("You Won!");
                            break;
                        case GAME_LOST:
                            GUI::message("You Lost!");
                            break;
                    }
                }
                break;

            case COMMS_MARK:
                // Play mark animation
                isDemarked = false;
                visualizer.addAnimation(markAnimation);
                break;
            
            case COMMS_DEMARK:
                // Play demark animation
                isDemarked = true;
                visualizer.addAnimation(markAnimation);
                break;

            default:
                // Unknown command, ignore
                break;
        }
        callRender = true;
    }

    // Redraw GUI if requested
    if (callRender) {
        GUI::callRender();
        callRender = false;
    }
}
