#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "GUI_Manager.hpp"
#include "Modules/Game.hpp"
#include <Arduino.h>

void againButtonCallback(ivec2 point, TouchStatus touchStatus);

class Gameplay : public Activity {
public:
    // Background and title elements
    Gradient background; // Background gradient element
    Text titleText; // Title text element
    Text narratorText; // Narrator text element

    // Player 1 elements
    Text player1Title;
    HpBar player1HpBar;

    // Player 2 elements
    Text player2Title;
    HpBar player2HpBar;

    // Additional elements
    Button againButton; // Button to restart the game

    // Variables
    int player1Hp = 100; // Player 1 health
    int player2Hp = 100; // Player 2 health
    int minHP; // Smallest HP of the two players
    int maxHP; // Maximum HP of the two players
    int leadingPlayer; // Leading player

    // Constructor
    Gameplay() : Activity(),
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
                "Play Again!", TFT_BLACK, TFT_YELLOW, TFT_BLACK, 20, 1,
                0.0f, &FreeMono18pt7b, true, true)
    {
        // Set touch event for the button
        againButton.OnTouch_setHandler(againButtonCallback);
        againButton.OnTouch_setEnable(true);

        // Add the health bar to the activity's elements list
        Element* elems[] = { &background, &titleText, &narratorText, &player1Title, &player1HpBar, &player2Title, &player2HpBar, &againButton };
        elements.addFromArray(elems, sizeof(elems) / sizeof(Element*));

    }

    Viewport render(const Viewport &viewport) override {
        player1Hp = Game::player1.getHP();
        player2Hp = Game::player2.getHP();

        minHP = min(player1Hp, player2Hp);
        maxHP = max(player1Hp, player2Hp);

        // Update the leading player based on HP
        if (player1Hp > player2Hp) {
            leadingPlayer = 1; // Player 1 is leading
        } else if (player2Hp > player1Hp) {
            leadingPlayer = 2; // Player 2 is leading
        } else {
            leadingPlayer = 0; // Both players have the same HP
        }

        player1HpBar.setValue(player1Hp);
        player2HpBar.setValue(player2Hp);

        titleText.content =  
            "Game is running!";
        
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

        if (minHP == 0) {
            // Disable the button if the game is over
            againButton.visible = true;
        } else {
            // Enable the button if the game is still on
            againButton.visible = false;
        }
        
        Viewport vp = Activity::render(viewport);
        // Additional rendering logic for the health bar can be added here if needed.
        return vp;
    }
};

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

#endif // GAMEPLAY_HPP