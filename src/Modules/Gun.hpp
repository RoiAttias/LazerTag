#ifndef GUN_HPP
#define GUN_HPP

#include <Arduino.h>

enum GunStatus
{
    NOT_READY,
    READY,
    SHOOTING,
    RELOADING
};

class Gun
{
public:
    // Constants
    const String name;
    const int damage;
    const int magazine;
    const int roundsPerMinute;
    const unsigned long reloadTime;
    const bool fullAuto;

    // Volatile variables
    volatile int ammo;
    volatile unsigned long lastShot;
    volatile unsigned long lastReload;
    volatile GunStatus status;

    // Multipliers
    float damageMultiplier = 1.0f;
    float fireRateMultiplier = 1.0f;
    float reloadTimeMultiplier = 1.0f;

    // Constructor
    /**
     * @brief Construct a new Gun object
     * @param name The name of the gun
     * @param damage The damage of the gun
     * @param magazine The magazine size of the gun
     * @param roundsPerMinute The rounds per minute of the gun
     * @param reloadTime The reload time of the gun in milliseconds
     * @param fullAuto Whether the gun is full auto or not, default is false
    */
    Gun(String name, int damage, int magazine, int roundsPerMinute, unsigned long reloadTime, bool fullAuto = false) :
        name(name),
        damage(damage),
        magazine(magazine),
        roundsPerMinute(roundsPerMinute),
        reloadTime(reloadTime),
        fullAuto(fullAuto),
        ammo(magazine),
        lastShot(0),
        lastReload(0),
        status(NOT_READY) 
    {}

    // Loop
    /**
     * @brief The loop function for the gun
     * This function is used to update the status of the gun
     * based on the time passed since the last shot or reload
     */
    void loop() {
        unsigned long currentMillis = millis();
        if (status == SHOOTING) {
            unsigned long timeBetweenShots = 60000 / unsigned long(roundsPerMinute * fireRateMultiplier);
            if (currentMillis - lastShot >= timeBetweenShots) {
                status = READY;
            }
        } else if (status == RELOADING) {
            if (currentMillis - lastReload >= unsigned long(reloadTime * reloadTimeMultiplier)) {
                ammo = magazine;
                status = READY;
            }
        }
    }

    // Getters
    int getAmmo() {
        return ammo;
    }

    GunStatus getStatus() {
        return status;
    }

    int getDamage() {
        return int(damage * damageMultiplier);
    }

    // Setters
    bool enable(bool newState) {
        if (newState && status == NOT_READY) {
            status = READY;
        } else {
            status = NOT_READY;
        }
        return newState;
    }

    // Methods
    /**
     * @brief Shoot the gun
     * @return Whether the gun was able to shoot or not
     */
    bool shoot() {
        if (status == READY && ammo > 0) {
            unsigned long currentMillis = millis();
            unsigned long timeBetweenShots = 60000 / (roundsPerMinute * fireRateMultiplier);
            if (currentMillis - lastShot >= timeBetweenShots) {
                status = SHOOTING;
                ammo--;
                lastShot = currentMillis;
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Reload the gun
     * @return Whether the gun was able to reload or not
     */
    bool reload() {
        if (status == READY && ammo < magazine) {
            status = RELOADING;
            lastReload = currentMillis;
            return true;
        }
        return false;
    }
};

class AssaultRifle : public Gun
{
public:
    AssaultRifle() : Gun("Assault Rifle", 20, 30, 600, 3000, true) {}
};

class Sidearm : public Gun
{
public:
    Sidearm() : Gun("Sidearm", 28, 10, 100, 2000, false) {}
};

class HandCannon : public Gun
{
public:
    HandCannon() : Gun("Hand Cannon", 35, 6, 70, 3000, false) {}
};

#endif // GUN_HPP