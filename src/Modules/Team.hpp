#ifndef TEAM_HPP
#define TEAM_HPP

#include <Arduino.h>

uint16_t teamIDs = 0;
int newTeamID()
{
    return ++teamIDs;
}
void resetTeamIDcounter() {
    teamIDs = 0;
}

class Team {
public:
    const uint16_t ID = newTeamID();
    String name;

    uint8_t color[3] = {0, 0, 0};

    int score = 0;

    Team(uint8_t teamID, String teamName = "null") : ID(teamID), name(teamName) {
        if (name == "null")
        {
            name = "Player";
            name += ID;
        }
    }
};

#endif // TEAM_HPP