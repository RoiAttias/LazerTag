#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Gun.hpp"

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

    uint8_t teamID;
    String name;

    MacAddress gunAddress;
    MacAddress vestAddress;
    
    int hp;
    Gun gun;


    Player(uint8_t teamID, String playerName = "null", MacAddress gunAddr = nullptr, MacAddress vestAddr = nullptr)
        : teamID(teamID), name(playerName), gunAddress(gunAddr), vestAddress(vestAddr)
    {
        if (name == "null")
        {
            name = "Player";
            name += ID;
        }

        hp = 0;
        gun = Sidearm();
    }

    // HP methods
    int getHP()
    {
        return hp;
    }

    void setHP(int health)
    {
        hp = health;
    }

    void damage(int damage)
    {
        hp -= damage;
    }


    // Gun methods
    uint32_t getGunDamage()
    {
        return gun.getDamage();
    }
};

#endif // PLAYER_HPP