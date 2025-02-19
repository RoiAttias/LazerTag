#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <
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

    int HP;

    uint8_t teamID;

    Player(uint8_t teamID, String playerName = "null") : teamID(teamID), name(playerName) {
        if (name == "null")
        {
            name = "Player";
            name += ID;
        }
    }
};

#endif // PLAYER_HPP