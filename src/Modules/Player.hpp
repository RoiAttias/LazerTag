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
    uint8_t ID;
    uint8_t teamID;
    String name;
    MacAddress gunAddress;
    MacAddress vestAddress;
    int hp;
    Gun gun;

    Player(uint8_t playerID ,uint8_t teamID = 0, String playerName = "null", MacAddress gunAddr = MacAddress(), MacAddress vestAddr = MacAddress())
        : ID(playerID), teamID(teamID), name(playerName), gunAddress(gunAddr), vestAddress(vestAddr), hp(100), gun(Sidearm)
    {
        ID = newPlayerID();
        if (name == "null")
        {
            name = "Player";
            name += ID;
        }
    }

    // ID methods
    uint8_t getID()
    {
        return ID;
    }

    uint8_t getTeamID()
    {
        return teamID;
    }

    void setTeamID(uint8_t teamID)
    {
        this->teamID = teamID;
    }

    String getName()
    {
        return name;
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

    GunData getGunData()
    {
        return gun.getData();
    }

    // MacAddress methods
    MacAddress getGunAddress()
    {
        return gunAddress;
    }

    MacAddress getVestAddress()
    {
        return vestAddress;
    }

    void setGunAddress(MacAddress gunAddr)
    {
        gunAddress = gunAddr;
    }

    void setVestAddress(MacAddress vestAddr)
    {
        vestAddress = vestAddr;
    }

    bool hasGun()
    {
        return gunAddress != MacAddress();
    }

    bool hasVest()
    {
        return vestAddress != MacAddress();
    }

    // Team methods
    uint8_t getTeamID()
    {
        return teamID;
    }
};

#endif // PLAYER_HPP