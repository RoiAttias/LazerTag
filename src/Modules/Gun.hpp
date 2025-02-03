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
    float damageMultiplier = 1.0;
    float fireRateMultiplier = 1.0;
    float reloadTimeMultiplier = 1.0;

    // Constructor
    Gun(String name, int damage, int magazine, int roundsPerMinute, unsigned long reloadTime, bool fullAuto) :
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

    // Methods
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

    bool reload() {
        if (status == READY && ammo < magazine) {
            status = RELOADING;
            lastReload = currentMillis;
            return true;
        }
        return false;
    }
};

class Sidearm : public Gun
{
};

#endif // GUN_HPP