#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Gun.hpp"
#include "Components/Nexus/Nexus.hpp"

class Player {
public:
    uint8_t ID;
    NexusAddress gunAddress;
    NexusAddress vestAddress;
    int hp;
    GunData gunData;

    Player(uint8_t playerID, NexusAddress gunAddr = NexusAddress(), NexusAddress vestAddr = NexusAddress())
        : ID(playerID), gunAddress(gunAddr), vestAddress(vestAddr), hp(100), gunData(Sidearm){}

    // ID methods
    uint8_t getID()
    {
        return ID;
    }

    void setID(uint8_t playerID)
    {
        ID = playerID;
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

    bool isAlive()
    {
        return hp > 0;
    }

    void resetHP()
    {
        hp = 100;
    }

    void damage(int damage)
    {
        hp = max(0, hp - damage);
    }

    // Gun methods
    uint32_t getGunDamage()
    {
        return gunData.damage;
    }

    GunData getGunData()
    {
        return gunData;
    }

    void setGunData(GunData data)
    {
        gunData = data;
    }

    // NexusAddress methods
    NexusAddress getGunAddress()
    {
        return gunAddress;
    }

    NexusAddress getVestAddress()
    {
        return vestAddress;
    }

    void setGunAddress(NexusAddress gunAddr)
    {
        gunAddress = gunAddr;
    }

    void setVestAddress(NexusAddress vestAddr)
    {
        vestAddress = vestAddr;
    }

    bool hasGun()
    {
        return gunAddress != NexusAddress();
    }

    bool hasVest()
    {
        return vestAddress != NexusAddress();
    }
};

#endif // PLAYER_HPP