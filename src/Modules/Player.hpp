/**
 * @file Player.hpp
 * @brief Represents a game player, tracking ID, health, weapon and vest addresses, and associated gun data.
 *
 * Provides methods to manage player state, health, and weapon configuration, as well as address lookup for Nexus communication.
 */

 #ifndef PLAYER_HPP
 #define PLAYER_HPP
 
 #include "Gun.hpp"                 ///< Defines GunData and gun loadouts
 #include "Components/Nexus/Nexus.hpp" ///< Defines NexusAddress for networking
 
 /**
  * @class Player
  * @brief Encapsulates a player’s identity, health, equipment addresses, and weapon configuration.
  *
  * Maintains player-specific information for health (HP), assigned gun and vest Nexus addresses,
  * and the GunData structure representing the player's current weapon setup.
  */
 class Player {
 public:
     uint8_t      ID;            ///< Unique player identifier (1..n)
     NexusAddress gunAddress;    ///< Network address of the player's gun module
     NexusAddress vestAddress;   ///< Network address of the player's vest module
     int          hp;            ///< Current health points (0 = dead)
     GunData      gunData;       ///< Current weapon configuration data
 
     /**
      * @brief Construct a Player with default health and Sidearm loadout.
      *
      * @param playerID  Numeric player ID
      * @param gunAddr   NexusAddress of the gun (default empty)
      * @param vestAddr  NexusAddress of the vest (default empty)
      */
     Player(uint8_t playerID,
            NexusAddress gunAddr = NexusAddress(),
            NexusAddress vestAddr = NexusAddress())
       : ID(playerID),
         gunAddress(gunAddr),
         vestAddress(vestAddr),
         hp(100),
         gunData(Sidearm) {}       
 
     // -------------------------------------------------------------------------
     // Identification
     // -------------------------------------------------------------------------
     /** @brief Get the player ID. */
     uint8_t getID() const { return ID; }
     /** @brief Set the player ID. */
     void setID(uint8_t playerID) { ID = playerID; }
 
     // -------------------------------------------------------------------------
     // Health Management
     // -------------------------------------------------------------------------
     /** @brief Get current health points. */
     int getHP() const { return hp; }
     /** @brief Set health points to a specific value. */
     void setHP(int health) { hp = health; }
     /** @brief Check if the player is alive (hp > 0). */
     bool isAlive() const { return hp > 0; }
     /** @brief Reset health points to full (100). */
     void resetHP() { hp = 100; }
     /**
      * @brief Apply damage to the player, clamped at zero.
      *
      * @param damage Amount of HP to subtract
      */
     void damage(int damage) { hp = max(0, hp - damage); }
 
     // -------------------------------------------------------------------------
     // Weapon Configuration
     // -------------------------------------------------------------------------
     /** @brief Get configured weapon damage value. */
     uint32_t getGunDamage() const { return gunData.damage; }
     /** @brief Retrieve the full GunData struct. */
     GunData getGunData() const { return gunData; }
     /** @brief Update the player’s weapon configuration. */
     void setGunData(const GunData& data) { gunData = data; }
 
     // -------------------------------------------------------------------------
     // Nexus Addresses
     // -------------------------------------------------------------------------
     /** @brief Get the Nexus address of the gun module. */
     NexusAddress getGunAddress() const { return gunAddress; }
     /** @brief Set the Nexus address for the gun module. */
     void setGunAddress(const NexusAddress& addr) { gunAddress = addr; }
 
     /** @brief Get the Nexus address of the vest module. */
     NexusAddress getVestAddress() const { return vestAddress; }
     /** @brief Set the Nexus address for the vest module. */
     void setVestAddress(const NexusAddress& addr) { vestAddress = addr; }
 
     /** @brief Check if a valid gun address is assigned. */
     bool hasGun() const { return gunAddress != NexusAddress(); }
     /** @brief Check if a valid vest address is assigned. */
     bool hasVest() const { return vestAddress != NexusAddress(); }
 };
 
 #endif // PLAYER_HPP 