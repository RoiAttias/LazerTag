/**
 * @file Gameplay.hpp
 * @brief Defines the Gameplay activity for the Manager device.
 *
 * The Gameplay activity displays the real-time state of the game:
 * - Two health bars (one per player)
 * - Dynamic narration text reflecting the current state
 * - A "Play Again!" button to reset the game once it ends
 */

#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP 

#include "GUI_Manager.hpp"   ///< LuminaUI screen, touch, and Nexus includes
#include "Modules/Game.hpp"  ///< Game state and logic
#include <Arduino.h>         ///< Arduino core

/**
 * @brief Callback invoked when the "Play Again!" button is pressed/released.
 *
 * If released, returns the UI to the activation screen to start a new match.
 *
 * @param point      The touch coordinates.
 * @param touchStatus The type of touch event (PRESS, RELEASE, READY, etc.).
 */
void againButtonCallback(ivec2 point, TouchStatus touchStatus);

/**
 * @brief Touch handler for Player 1 name label and HP bar.
 *
 * If pressed, sends a "mark" packet to the respective device.
 *
 * @param point      The touch coordinates.
 * @param touchStatus The type of touch event (PRESS, RELEASE, READY, etc.).
 */
void markPlayer1(ivec2 point, TouchStatus touchStatus);

/**
 * @brief Touch handler for Player 2 name label.
 *
 * If pressed, sends a "mark" packet to the respective device.
 *
 * @param point      The touch coordinates.
 * @param touchStatus The type of touch event (PRESS, RELEASE, READY, etc.).
 */
void markPlayer2(ivec2 point, TouchStatus touchStatus);

/**
 * @class Gameplay
 * @brief A LuminaUI Activity showing the running game dashboard for the manager.
 *
 * This activity renders:
 *  - A gradient background
 *  - Static title "Game is running!"
 *  - A dynamic narrator text that changes based on players' health
 *  - Two health bars: one for Player 1 and one for Player 2
 *  - A "Play Again!" button once the game concludes
 */
class Gameplay : public Activity {
public:
    Background background;   ///< Gradient background element
    Text       titleText;    ///< Static title at the top
    Text       narratorText; ///< Dynamic narration text

    Text  player1Title;   ///< Label for Player 1
    HpBar player1HpBar;   ///< Health bar for Player 1

    Text  player2Title;   ///< Label for Player 2
    HpBar player2HpBar;   ///< Health bar for Player 2

    Button againButton;   ///< Button to restart the game after it ends

    // Internal state tracking
    int player1Hp;     ///< Current HP of Player 1
    int player2Hp;     ///< Current HP of Player 2
    int minHP;         ///< Minimum of the two HP values
    int maxHP;         ///< Maximum of the two HP values
    int leadingPlayer; ///< ID of the leading player (1 or 2), or 0 if tied

    /**
     * @brief Construct a new Gameplay activity.
     *
     * Initializes all UI elements, positions them, and sets up the touch handler
     * for the "Play Again!" button.
     */
    Gameplay()
      : Activity(),
        background(), 
        titleText(Element(ivec2(0, 10), LuminaUI_AUTO, ivec2(480, 50)),
                  String("Game is running!"), TFT_WHITE, 1, MC_DATUM, 0.0f, &FreeMonoBold24pt7b),
        narratorText(Element(ivec2(0, 70), LuminaUI_AUTO, ivec2(480, 70)),
                     String("Game Started!"), TFT_WHITE, 1, TC_DATUM, 1.1f, &FreeMono9pt7b),
        player1Title(Element(ivec2(0, 130), LuminaUI_AUTO, ivec2(240, 40)),
                     String("Player 1"), TFT_WHITE, 1, MC_DATUM, 0.0f, &FreeMonoBold18pt7b),
        player1HpBar(Element(ivec2(20, 180), LuminaUI_AUTO, ivec2(200, 50))),
        player2Title(Element(ivec2(240, 130), LuminaUI_AUTO, ivec2(240, 40)),
                     String("Player 2"), TFT_WHITE, 1, MC_DATUM, 0.0f, &FreeMonoBold18pt7b),
        player2HpBar(Element(ivec2(260, 180), LuminaUI_AUTO, ivec2(200, 50))),
        againButton(Element(ivec2(90, 240), LuminaUI_AUTO, ivec2(300, 70), false),
                    "Play Again!", TFT_BLACK, TFT_YELLOW, TFT_BLACK, 20, 1, 0.0f, &FreeMono18pt7b, true, true),
        player1Hp(100),
        player2Hp(100),
        minHP(100),
        maxHP(100),
        leadingPlayer(0)
    {
        // Enable touch for the restart button and assign its handler
        againButton.OnTouch_setHandler(againButtonCallback);
        againButton.OnTouch_setEnable(true);

        // Enable touch for player name labels and HP bars, and assign their handlers
        player1Title.OnTouch_setHandler(markPlayer1);
        player1Title.OnTouch_setEnable(true);
        player2Title.OnTouch_setHandler(markPlayer2);
        player2Title.OnTouch_setEnable(true);
        player1HpBar.OnTouch_setHandler(markPlayer1);
        player1HpBar.OnTouch_setEnable(true);
        player2HpBar.OnTouch_setHandler(markPlayer2);
        player2HpBar.OnTouch_setEnable(true);

        // Add all elements to the activity's rendering list
        Element* elems[] = {
            &background,
            &titleText,
            &narratorText,
            &player1Title,
            &player1HpBar,
            &player2Title,
            &player2HpBar,
            &againButton
        };
        elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));
    }

    /**
     * @brief Renders and updates the gameplay UI.
     *
     * - Reads current HP from Game module.
     * - Determines leading player or tie.
     * - Selects an appropriate narration message.
     * - Shows the "Play Again!" button if the game has ended.
     * - Calls the base class to perform actual drawing.
     *
     * @param viewport The clipping viewport for rendering.
     * @return The footer-clamped viewport after rendering.
     */
    virtual Viewport render(const Viewport &viewport) override {
        // Fetch latest health values from game state
        player1Hp = Game::player1.getHP();
        player2Hp = Game::player2.getHP();

        // Compute min/max HP and leading player
        minHP = min(player1Hp, player2Hp);
        maxHP = max(player1Hp, player2Hp);
        if      (player1Hp > player2Hp) leadingPlayer = 1;
        else if (player2Hp > player1Hp) leadingPlayer = 2;
        else                             leadingPlayer = 0;

        // Update the health bars
        player1HpBar.setValue(player1Hp);
        player2HpBar.setValue(player2Hp);

        // Narration for key game events with added dynamic sentences
        if (maxHP == 100 && leadingPlayer == 0) {
            narratorText.content =  
                "Game Started! Let the duel commence. Prepare for an epic showdown on the laser taggin' battlefield!";
            /** Another options:
             * "As the two competitors arrives .The arena is charged with anticipation as they prepare for the ultimate showdown!"
             * "The arena is alive with energy! Both players are at full health, ready to unleash their skills in this electrifying duel!"
             */
        } else if (minHP == 0 && leadingPlayer != 0) {
            titleText.content =  
                "Player " + String(leadingPlayer) + " wins!";
            narratorText.content =  "Game's Over! Player " + String(leadingPlayer)
                + " wins! What an electrifying duel that kept us on the edge of our seats!";
        } else if (minHP == 0 && leadingPlayer == 0) {
            titleText.content =  
                "It's a tie!";
            narratorText.content =  
                "It's a tie! Both players fought valiantly, but the arena has claimed them both!";
        } else if (maxHP == 100 && leadingPlayer != 0) {
            narratorText.content =  
                "Player " + String(leadingPlayer) + " is blazing ahead! With lightning-fast moves, the arena ignites with energy!";
        } else if (minHP > 60 && minHP < 100 && leadingPlayer != 0) {
            narratorText.content =  
                "Player " + String(leadingPlayer) + " is in the lead! The tension is palpable as the duel intensifies!";
        } else if (minHP <= 60 && minHP > 30) {
            narratorText.content =  
                "The duel is heating up!";
        } else if (minHP <= 30 && minHP > 15) {
            narratorText.content =  
                "The stakes are high! One wrong move could turn the tide of battle!";
        } else if (minHP <= 15 && leadingPlayer != 0) {
            narratorText.content =  
                "Player " + String((leadingPlayer % 2) + 1) + " is hanging by a thread! One more hit could change everything!";
        } else {
            narratorText.content =  
                "The duel rages on... Every shot is a heartbeat, and the tension is lighting up the arena!";
        }

        // Show or hide the restart button based on game over
        againButton.visible = (minHP == 0);

        // Delegate actual drawing to the base Activity
        return Activity::render(viewport);
    }
};

/**
 * @brief Global instance of the Gameplay activity.
 *        Used by GUI_Manager to switch screens.
 */
Gameplay* gameplay = new Gameplay();

void againButtonCallback(ivec2 point, TouchStatus status) {
    if (status == TouchStatus_RELEASE) {
        GUI::selectActivity(GUI_Manager_Activity::ACTIVATION);
    }
    else if (status == TouchStatus_PRESS) {
        // Change the button color when pressed
        gameplay->againButton.background.fillColor = TFT_MAROON;
        gameplay->againButton.background.borderColor = TFT_YELLOW;
        gameplay->againButton.text.textColor = TFT_YELLOW;
        gameplay->againButton.callRender();
    }
    else if (status == TouchStatus_READY) {
        // Change the button color back when released
        gameplay->againButton.background.fillColor = TFT_BLACK;
        gameplay->againButton.background.borderColor = TFT_YELLOW;
        gameplay->againButton.text.textColor = TFT_YELLOW;
        gameplay->againButton.callRender();
    }
}

void markPlayer1(ivec2, TouchStatus status) {
    if (status == TouchStatus_PRESS) {
        if (Game::player1.hasGun()) {
            Nexus::sendData(COMMS_MARK, 0, nullptr,
                        Game::player1.getGunAddress());
        }
        if (Game::player1.hasVest()) {
            Nexus::sendData(COMMS_MARK, 0, nullptr,
                        Game::player1.getVestAddress());
        }
    }
}

void markPlayer2(ivec2, TouchStatus status) {
    if (status == TouchStatus_PRESS) {
        if (Game::player2.hasGun()) {
            Nexus::sendData(COMMS_MARK, 0, nullptr,
                        Game::player2.getGunAddress());
        }
        if (Game::player2.hasVest()) {
            Nexus::sendData(COMMS_MARK, 0, nullptr,
                        Game::player2.getVestAddress());
        }
    }
}

#endif // GAMEPLAY_HPP