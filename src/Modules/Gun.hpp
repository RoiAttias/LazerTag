/**
 * @file Gun.hpp
 * @brief Defines the Gun class and related data structures for simulating weapon behavior.
 *
 * Provides:
 *  - GunStatus enum for weapon states (NOT_READY, READY, SHOOTING, RELOADING).
 *  - Packed GunData struct for sending/receiving gun configuration over comms.
 *  - Gun class implementing shooting, reloading, rate-of-fire handling, and callbacks via Countdowner.
 *  - Predefined GunData constants (Stinger, Ghost, Hammerfall) for common weapon loadouts.
 */

 #ifndef GUN_HPP
 #define GUN_HPP
 
 #include <Arduino.h>
 #include "Utilities/Countdowner.hpp"  ///< Timer utility for scheduling burst shots
 
#define MAX_GUN_NAME_LENGTH 32  ///< Maximum length for gun name strings
#define MAX_GUN_DESCRIPTION_LENGTH 256  ///< Maximum length for gun description strings

 /**
  * @enum GunStatus
  * @brief Represents the current operational state of a Gun.
  */
 enum GunStatus {
     NOT_READY,  ///< Gun is disabled or uninitialized
     READY,      ///< Gun is ready to fire
     SHOOTING,   ///< Gun is actively firing
     RELOADING   ///< Gun is reloading its magazine
 };
 
 /**
  * @struct GunData
  * @brief Packed data structure representing a gun's configuration.
  *
  * Used for serialization over communication channels (e.g., Nexus). All fields
  * are laid out without padding.
  */
 struct __attribute__((packed)) GunData {
     uint32_t damage;         ///< Damage per shot
     uint32_t magazine;       ///< Number of rounds per magazine
     uint32_t roundsPerMinute;///< Rate of fire (RPM)
     uint32_t reloadTime;     ///< Reload duration in milliseconds
     bool     fullAuto;       ///< True if full-auto mode enabled
     uint8_t  burst;          ///< Number of shots per trigger pull
     uint32_t burstInterval;  ///< Interval between burst shots in milliseconds
 
     /**
      * @brief Get a human-readable summary of the gun configuration.
      * @return Formatted string with all GunData fields.
      */
     String toString() const {
         String str = "";
         str += "Damage: " + String(damage) + "\n";
         str += "Magazine: " + String(magazine) + "\n";
         str += "RPM: " + String(roundsPerMinute) + "\n";
         str += "ReloadTime: " + String(reloadTime) + "ms\n";
         str += "FullAuto: " + (fullAuto ? String("True") : String("False")) + "\n";
         str += "Burst: " + String(burst) + "\n";
         str += "BurstInterval: " + String(burstInterval) + "ms\n";
         return str;
     }
 };
 
 /**
  * @class Gun
  * @brief Simulates a firearm with ammo, firing rate, and reload mechanics.
  *
  * Manages internal state transitions between READY, SHOOTING, and RELOADING.
  * Supports single shot, burst fire, and full-auto based on configuration.
  * Uses Countdowner to schedule burst intervals.
  */
 class Gun {
 public:
     // Configuration parameters
     uint32_t damage;             ///< Base damage per shot
     uint32_t magazine;           ///< Magazine capacity
     uint32_t roundsPerMinute;    ///< Fire rate in RPM
     uint32_t reloadTime;         ///< Reload duration (ms)
     bool     fullAuto;           ///< Full-auto mode flag
     uint8_t  burst;              ///< Shots per burst (1 = single shot)
     uint32_t burstInterval;      ///< Delay between burst shots (ms)
 
     // Runtime variables
     uint32_t ammo;               ///< Current ammo in magazine
     uint32_t lastShot;           ///< Timestamp of last shot (ms)
     uint32_t lastReload;         ///< Timestamp when reload started (ms)
     GunStatus status;            ///< Current gun status
 
     // Event callbacks
     void (*onReloadFinish)(Gun *gun) = nullptr; ///< Called when reload completes
     void (*countdowner_shoot)(int) = nullptr;   ///< Called by Countdowner for burst shots
 
     // Modifiers
     float damageMultiplier = 1.0f;      ///< Multiplies base damage
     float fireRateMultiplier = 1.0f;    ///< Multiplies fire rate
     float reloadTimeMultiplier = 1.0f;  ///< Multiplies reload duration
 
     /**
      * @brief Construct a Gun with explicit parameters.
      *
      * @param damage            Damage per shot
      * @param magazine          Magazine size
      * @param roundsPerMinute   Rate of fire (RPM)
      * @param reloadTime        Reload duration (ms)
      * @param fullAuto          Enable full-auto mode
      * @param burst             Burst shot count
      * @param burstInterval     Interval between burst shots (ms)
      * @param countdownerShoot  Callback for scheduled burst shots
      * @param onReloadFinish    Callback when reload completes
      */
     Gun(uint32_t damage, uint32_t magazine, uint32_t roundsPerMinute, uint32_t reloadTime,
         bool fullAuto = false, uint8_t burst = 1, uint32_t burstInterval = 0,
         void (*countdownerShoot)(int) = nullptr, void (*onReloadFinishFunc)(Gun *gun) = nullptr)
       : damage(damage),
         magazine(magazine),
         roundsPerMinute(roundsPerMinute),
         reloadTime(reloadTime),
         fullAuto(fullAuto),
         ammo(0),
         lastShot(0),
         lastReload(0),
         status(NOT_READY),
         countdowner_shoot(countdownerShoot),
         onReloadFinish(onReloadFinishFunc) {}
 
     /**
      * @brief Construct a Gun from a GunData struct.
      *
      * @param data               GunData instance
      * @param countdownerShoot   Callback for scheduled burst shots
      * @param onReloadFinishFunc Callback when reload completes
      */
     Gun(const GunData& data, void (*countdownerShoot)(int) = nullptr,
         void (*onReloadFinishFunc)(Gun *gun) = nullptr)
       : Gun(data.damage, data.magazine, data.roundsPerMinute, data.reloadTime,
             data.fullAuto, data.burst, data.burstInterval,
             countdownerShoot, onReloadFinishFunc) {}
 
     /**
      * @brief Update method; advances state based on timers.
      *
      * - If SHOOTING and interval elapsed: set READY
      * - If RELOADING and reloadTime elapsed: refill ammo, set READY, invoke callback
      */
     void loop() {
         uint32_t now = millis();
         if (status == SHOOTING) {
             uint32_t interval = 60000u / uint32_t(roundsPerMinute * fireRateMultiplier);
             if (now - lastShot >= interval) status = READY;
         } else if (status == RELOADING) {
             if (now - lastReload >= uint32_t(reloadTime * reloadTimeMultiplier)) {
                 ammo = magazine;
                 status = READY;
                 if (onReloadFinish) onReloadFinish(this);
             }
         }
     }
 
     /**
      * @brief Attempt to fire the gun.
      *
      * If READY and ammo available, transitions to SHOOTING, records timestamp,
      * triggers immediate callback, and schedules subsequent burst shots.
      */
     void shoot() {
         if (status == READY && ammo > 0) {
             uint32_t now = millis();
             uint32_t interval = 60000u / (roundsPerMinute * fireRateMultiplier);
             if (now - lastShot >= interval) {
                 status = SHOOTING;
                 lastShot = now;
                 if (countdowner_shoot) {
                     countdowner_shoot(0);  // immediate shot
                     for (int i = 1; i < burst; ++i) {
                         countdowner->addEvent(i * burstInterval, countdowner_shoot, i);
                     }
                 }
             }
         }
     }
 
     /**
      * @brief Decrease ammo by one (floor at zero).
      */
     void decreaseAmmo() {
         if (ammo > 0) --ammo;
     }
 
     /**
      * @brief Cancel shooting state and return to READY.
      */
     void release() {
         if (status == SHOOTING) status = READY;
     }
 
     /**
      * @brief Begin reload sequence if READY and magazine not full.
      * @return True if reload started, false otherwise
      */
     bool reload() {
         if (status == READY && ammo != magazine) {
             status = RELOADING;
             lastReload = millis();
             return true;
         }
         return false;
     }
 
     // -------------------------------------------------------------------------
     // Getters and Setters
     // -------------------------------------------------------------------------
     void setGunData(const GunData& data) {
         damage = data.damage;
         magazine = data.magazine;
         roundsPerMinute = data.roundsPerMinute;
         reloadTime = data.reloadTime;
         fullAuto = data.fullAuto;
         burst = data.burst;
         burstInterval = data.burstInterval;
     }


     /** @brief Enable or disable the gun (sets status accordingly).
      *  @return The new enable state
      */
     bool enable(bool newState) {
         status = newState ? READY : NOT_READY;
         return newState;
     }
 
     /** @brief Set gun status to READY.
      */
     void start() { status = READY; }
 
     /** @brief Set gun status to NOT_READY.
      */
     void end()   { status = NOT_READY; }
 
     /** @brief Get current ammo count. */
     uint32_t getAmmo() const { return ammo; }
 
     /** @brief Get magazine capacity. */
     uint32_t getMagazine() const { return magazine; }
 
     /** @brief Get current gun status. */
     GunStatus getStatus() const { return status; }
 
     /** @brief Get effective damage (with multiplier). */
     uint32_t getDamage() const { return uint32_t(damage * damageMultiplier); }
 
     /**
      * @brief Set callbacks for reload finish and burst scheduling.
      */
     void setCountdownerShootCallback(void (*callback)(int)) { countdowner_shoot = callback; }
     void setOnReloadFinishCallback(void (*callback)(Gun *)) { onReloadFinish = callback; }
 };

// --------------------------------------------------------------------------
// Gun Setups
// --------------------------------------------------------------------------
 
 /**
  * @brief Predefined loadout: "Stinger" burst pistol.
  *
  * Statistics:
  *  - Damage: 10
  *  - Magazine: 18 rounds
  *  - Rate: 80 RPM
  *  - Reload: 1800 ms
  *  - Full-auto: false
  *  - Burst: 3 shots
  *  - Burst Interval: 80 ms
  */
const GunData Stinger = {10, 18, 80, 1800, false, 3, 80};
 
 /**
  * @brief Predefined loadout: "Ghost" full-auto pistol.
  *
  * Statistics:
  *  - Damage: 7
  *  - Magazine: 13 rounds
  *  - Rate: 500 RPM
  *  - Reload: 1500 ms
  *  - Full-auto: true
  *  - Burst: 1 (single shot)
  *  - Burst Interval: 0 ms
  */
const GunData Ghost = {7, 13, 500, 1500, true, 1, 0};
 
 /**
  * @brief Predefined loadout: "Hammerfall" heavy pistol.
  *
  * Statistics:
  *  - Damage: 26
  *  - Magazine: 8 rounds
  *  - Rate: 150 RPM
  *  - Reload: 3000 ms
  *  - Full-auto: false
  * - Burst: 1 (single shot)
  *  - Burst Interval: 0 ms
  */
const GunData Hammerfall = {26, 8, 150, 3000, false, 1, 0};

const GunData gunDataArray[] = {
    Stinger,
    Ghost,
    Hammerfall
};
constexpr int gunDataArraySize = sizeof(gunDataArray) / sizeof(GunData);
const char gunDataNameArray[gunDataArraySize][MAX_GUN_NAME_LENGTH] = {
    "Stinger",
    "Ghost",
    "Hammerfall"
};
const char gunDataDescriptionArray[gunDataArraySize][MAX_GUN_DESCRIPTION_LENGTH] = {
    "A 3-shot burst pistol.\n\"Ta! Ta! Ta! [Taking breath] Ta! Ta! Ta! [Taking breath] Ta! Ta! Ta!\"",
    "A full-auto pistol.\n\"Spray and pray! Ha Ha Ha!\"\nYou\'re gonna love this one.",
    "A heavy pistol dealing a lot of damage, in the costs of low RPM and slow reload time.\n\"KA-BOOM!\""
};
 
 /**
  * @brief Get the GunData for a specific gun index.
  * @param index Index of the gun in the array
  * @return GunData instance
  */
 inline const GunData& getGunData(int index) {
     if (index < 0 || index >= gunDataArraySize) {
         return gunDataArray[0]; // Return default if out of bounds
     }
     return gunDataArray[index];
 }

#endif // GUN_HPP