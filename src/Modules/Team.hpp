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
const uint16_t ID = newTeamID();
String name;

int HP;

int score;
int kills;
};

#endif // TEAM_HPP