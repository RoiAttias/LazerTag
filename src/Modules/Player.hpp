#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Gun.hpp"
#include "Utilities/MacAddress.hpp"

static uint16_t playerIDs = 0;

int newPlayerID()
{
    return ++playerIDs;
}

void resetPlayerIDcounter() {
    playerIDs = 0;
}

class Player {
public:
    uint16_t ID;
    uint8_t teamID;
    String name;
    MacAddress gunAddress;
    MacAddress vestAddress;
    int hp;
    Gun gun;

    Player(uint8_t teamID = 0, String playerName = "null", MacAddress gunAddr = MacAddress(), MacAddress vestAddr = MacAddress())
        : teamID(teamID), name(playerName), gunAddress(gunAddr), vestAddress(vestAddr), hp(100), gun(Sidearm)
    {
        ID = newPlayerID();
        if (name == "null")
        {
            name = "Player";
            name += ID;
        }
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