#ifndef GUN_HPP
#define GUN_HPP

#include <Arduino.h>
#include <stdint.h> // Include the standard integer types

/**
 * @brief An enum representing the status of a Gun
 */
enum GunStatus
{
    NOT_READY,
    READY,
    SHOOTING,
    RELOADING
};

/**
 * @brief A struct representing the data of a Gun
 * This struct is used to store the data of a Gun in a packed format to send over the comms.
 */
struct __attribute__((packed)) GunData
{
    uint32_t damage; // 4 bytes
    uint32_t magazine; // 4 bytes
    uint32_t roundsPerMinute; // 4 bytes
    uint32_t reloadTime; // 4 bytes
    bool fullAuto; // 1 byte
};

/**
 * @brief A base class representing a Gun
 * This class initializes a gun with specific attributes and provides methods to shoot and reload the gun.
 */
class Gun
{
public:
    // Constants
    const String name;
    const uint32_t damage;
    const uint32_t magazine;
    const uint32_t roundsPerMinute;
    const uint32_t reloadTime;
    const bool fullAuto;

    // Variables
    uint32_t ammo;
    uint32_t lastShot;
    uint32_t lastReload;
    GunStatus status;

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
    Gun(String name, uint32_t damage, uint32_t magazine, uint32_t roundsPerMinute, uint32_t reloadTime, bool fullAuto = false) :
        name(name),
        damage(damage),
        magazine(magazine),
        roundsPerMinute(roundsPerMinute),
        reloadTime(reloadTime),
        fullAuto(fullAuto),
        ammo(0),
        lastShot(0),
        lastReload(0),
        status(NOT_READY)
    {}

    /**
     * @brief Construct a new Gun object
     * @param data The GunData object containing the data of the gun
    */
    Gun(GunData data) : Gun("Gun", data.damage, data.magazine, data.roundsPerMinute, data.reloadTime, data.fullAuto){}

    // Loop
    /**
     * @brief The loop function for the gun
     * This function is used to update the status of the gun
     * based on the time passed since the last shot or reload
     */
    void loop() {
        uint32_t currentMillis = millis();
        if (status == SHOOTING) {
            uint32_t timeBetweenShots = 60000 / uint32_t(roundsPerMinute * fireRateMultiplier);
            if (currentMillis - lastShot >= timeBetweenShots) {
                status = READY;
            }
        } else if (status == RELOADING) {
            if (currentMillis - lastReload >= uint32_t(reloadTime * reloadTimeMultiplier)) {
                ammo = magazine;
                status = READY;
            }
        }
    }

    // Getters
    GunData getData() {
        GunData data = {
            damage,
            magazine,
            roundsPerMinute,
            reloadTime,
            fullAuto
        };
        return data;
    }

    uint32_t getAmmo() {
        return ammo;
    }

    uint32_t getMagazine() {
        return magazine;
    }

    GunStatus getStatus() {
        return status;
    }

    uint32_t getDamage() {
        return uint32_t(damage * damageMultiplier);
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
     * @brief Start the gun by setting the status to READY
     */
    void start() {
        status = READY;
    }

    /**
     * @brief End the gun by setting the status to NOT_READY
     */
    void end() {
        status = NOT_READY;
    }

    /**
     * @brief Shoot the gun
     * @return Whether the gun was able to shoot or not
     */
    bool shoot() {
        if (status == READY && ammo > 0) {
            uint32_t currentMillis = millis();
            uint32_t timeBetweenShots = 60000 / (roundsPerMinute * fireRateMultiplier);
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
        uint32_t currentMillis = millis();
        if (status == READY && ammo < magazine) {
            status = RELOADING;
            lastReload = currentMillis;
            return true;
        }
        return false;
    }
};

/**
 * @brief A derived class representing a Sidearm
 * This class inherits from Gun and initializes a sidearm with specific attributes.
 * The attributes are as follows:
 * Name: Sidearm
 * Damage: 23
 * Magazine: 16
 * Rounds per minute: 300
 * Reload time: 1800ms
 * Full auto: true
 */
class Sidearm : public Gun
{
public:
    Sidearm() : Gun("Sidearm", 23, 16, 300, 1800, true) {}
};

/**
 * @brief A derived class representing a Hand Cannon
 * This class inherits from Gun and initializes a Hand Cannon with specific attributes.
 * The attributes are as follows:
 * Name: Hand Cannon
 * Damage: 42
 * Magazine: 8
 * Rounds per minute: 150
 * Reload time: 3000ms
 * Full auto: false
 */
class HandCannon : public Gun
{
public:
    HandCannon() : Gun("Hand Cannon", 42, 8, 150, 3000, false) {}
};

#endif // GUN_HPP