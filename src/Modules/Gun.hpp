#ifndef GUN_HPP
#define GUN_HPP

#include <Arduino.h>
#include "Utilities/Countdowner.hpp"

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
    uint8_t burst; // 1 byte
    uint32_t burstInterval; // 4 bytes

    String toString() {
        String str = "GunData: ";
        str += "Damage: " + String(damage) + "\n";
        str += "Magazine: " + String(magazine) + "\n";
        str += "Rounds per minute: " + String(roundsPerMinute) + "\n";
        str += "Reload time: " + String(reloadTime) + "\n";
        str += "Full auto: " + String(fullAuto) + "\n";
        str += "Burst: " + String(burst) + "\n";
        str += "Burst interval: " + String(burstInterval) + "\n";
        return str;
    }
};

class Gun;

/**
 * @brief A base class representing a Gun
 * This class initializes a gun with specific attributes and provides methods to shoot and reload the gun.
 */
class Gun
{
public:
    // Constants
    uint32_t damage;
    uint32_t magazine;
    uint32_t roundsPerMinute;
    uint32_t reloadTime;
    bool fullAuto;
    uint8_t burst = 1;
    uint32_t burstInterval = 0;

    // Variables
    uint32_t ammo;
    uint32_t lastShot;
    uint32_t lastReload;
    GunStatus status;

    // Events
    void (*onReloadFinish)(Gun *gun) = nullptr;
    void (*countdowner_shoot)(int) = nullptr;

    // Multipliers
    float damageMultiplier = 1.0f;
    float fireRateMultiplier = 1.0f;
    float reloadTimeMultiplier = 1.0f;

    // Constructor
    /**
     * @brief Construct a new Gun object
     * @param damage The damage of the gun
     * @param magazine The magazine size of the gun
     * @param roundsPerMinute The rounds per minute of the gun
     * @param reloadTime The reload time of the gun in milliseconds
     * @param fullAuto Whether the gun is full auto or not, default is false
     * @param burst The number of shots to fire in a burst, default is 1
     * @param burstInterval The interval between shots in a burst, default is 0
     * @param countdownerShootFunc The function to call when the gun is shot
     * @param onReloadFinishFunc The function to call when the gun is reloaded
    */
    Gun(uint32_t damage, uint32_t magazine, uint32_t roundsPerMinute, uint32_t reloadTime, bool fullAuto = false,
        uint8_t burst = 1, uint32_t burstInterval = 0,
        void (*countdownerShootFunc)(int) = nullptr, void (*onReloadFinishFunc)(Gun *gun) = nullptr):
        damage(damage),
        magazine(magazine),
        roundsPerMinute(roundsPerMinute),
        reloadTime(reloadTime),
        fullAuto(fullAuto),
        ammo(0),
        lastShot(0),
        lastReload(0),
        status(NOT_READY),
        countdowner_shoot(countdownerShootFunc),
        onReloadFinish(onReloadFinishFunc)
    {}

    /**
     * @brief Construct a new Gun object
     * @param data The GunData object containing the data of the gun
     * @param countdownerShootFunc The function to call when the gun is shot
     * @param onReloadFinishFunc The function to call when the gun is reloaded
    */
    Gun(GunData data, void (*countdownerShootFunc)(int) = nullptr, void (*onReloadFinishFunc)(Gun *gun) = nullptr)
        : Gun(data.damage, data.magazine, data.roundsPerMinute, data.reloadTime, data.fullAuto, data.burst, data.burstInterval,
        countdownerShootFunc, onReloadFinishFunc){}

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

                if (onReloadFinish) {
                    onReloadFinish(this);
                }
            }
        }
    }

    void setCountdownerShootCallback(void (*callback)(int)) {
        countdowner_shoot = callback;
    }

    void setOnReloadFinishCallback(void (*callback)(Gun *gun)) {
        onReloadFinish = callback;
    }    

    GunData getData() {
        GunData data = {
            damage,
            magazine,
            roundsPerMinute,
            reloadTime,
            fullAuto,
            burst,
            burstInterval
        };
        return data;
    }

    void setData(GunData data) {
        damage = data.damage;
        magazine = data.magazine;
        roundsPerMinute = data.roundsPerMinute;
        reloadTime = data.reloadTime;
        fullAuto = data.fullAuto;
        burst = data.burst;
        burstInterval = data.burstInterval;
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
     */
    void shoot() {
        if (status == READY && ammo > 0) {
            uint32_t currentMillis = millis();
            uint32_t timeBetweenShots = 60000 / (roundsPerMinute * fireRateMultiplier);
            if (currentMillis - lastShot >= timeBetweenShots) {
                status = SHOOTING;
                lastShot = currentMillis;

                if (countdowner_shoot) {
                    countdowner_shoot(0); // Call the shoot function immediately
                    for (int i = 1; i < burst; i++) {
                        countdowner->addEvent(i * burstInterval, countdowner_shoot, i);
                    }
                }
            }
        }
    }

    void decreaseAmmo() {
        if (ammo > 0) {
            ammo--;
        } else {
            ammo = 0;
        }
    }

    void release() {
        if (status == SHOOTING) {
            status = READY;
        }
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
 * Name: Sidearm
 * Damage: 13
 * Magazine: 18
 * Rounds per minute: 100
 * Reload time: 1800ms
 * Full auto: true
 * Burst: 3
 * Burst interval: 100ms
 */
const GunData Sidearm = {10, 18, 60, 1800, true, 3, 100};
/**
 * Name: Sidearm 2
 * Damage: 26
 * Magazine: 13
 * Rounds per minute: 300
 * Reload time: 1500ms
 * Full auto: true
 * Burst: 1
 * Burst interval: 0ms
 */
const GunData Sidearm_2 = {20, 13, 300, 1500, true, 1, 0};

/**
 * Name: Hand Cannon
 * Damage: 42
 * Magazine: 8
 * Rounds per minute: 150
 * Reload time: 3000ms
 * Full auto: false
 * Burst: 1
 * Burst interval: 0ms
 */
const GunData HandCannon = {30, 8, 150, 3000, false, 1, 0};

#endif // GUN_HPP