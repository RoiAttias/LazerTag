#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <Arduino.h>

uint16_t playerIDs = 0;
int newPlayerID()
{
    return ++playerIDs;
}
void resetPlayerIDcounter() {
    playerIDs = 0;
}

class Player {
public:
    const uint16_t ID = newPlayerID();
    String name;

    // 2 mac adresses
    // gun pointer
    int HP;

    uint16_t teamID;

    int score;
    int kills;

    int TotalDamageInput;
    int TotalDamageOutput;

    Player(uint16_t teamID, String playerName = "null") : teamID(teamID), name(playerName) {
        if (name == "null")
        {
            name = "Player";
            name += ID;
        }
    }
};

#endif // PLAYER_HPP