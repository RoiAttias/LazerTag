/**
 * @file LazertagPacket.hpp
 * @brief Definitions for Lazertag communication packet types and payload sizing.
 *
 * Declares the CommsCommand enum for different packet types sent over the Nexus network,
 * and provides a lookup array `payloadSizePerCommand` indicating the expected payload size
 * in bytes for each command. Includes game, player, gun, and Nexus modules.
 */

 #ifndef LAZERTAGPACKET_HPP
 #define LAZERTAGPACKET_HPP
 
 #include "Modules/Game.hpp"      ///< Defines GameStatus struct
 #include "Modules/Player.hpp"    ///< Defines Player state data
 #include "Modules/Gun.hpp"       ///< Defines GunData struct
 #include "Components/Nexus/Nexus.hpp" ///< Nexus packet layer
 
 #include "Constants_common.h"    ///< Common constants and macros
 
 /**
  * @enum CommsCommand
  * @brief Identifiers for packet commands in LaserTag game communication.
  *
  * Enumerates the different types of packets exchanged:
  *  - COMMS_PLAYERHP:   Transmit player health (int)
  *  - COMMS_GUNPARAMS:  Transmit gun parameters (GunData)
  *  - COMMS_FIRECODE:   Transmit fire event code (uint32_t)
  *  - COMMS_GAMESTATUS: Transmit overall game status (GameStatus)
  *  - COMMS_MARK:       Transmit hit marker (no payload)
  *  - COMMS_size:       Sentinel value for command count
  */
 enum CommsCommand : uint32_t {
     COMMS_PLAYERHP,   ///< Player health update
     COMMS_GUNPARAMS,  ///< Gun configuration parameters
     COMMS_FIRECODE,   ///< Fire event identifier
     COMMS_GAMESTATUS, ///< Game status update
     COMMS_MARK,       ///< Mark marker (no payload)
     COMMS_DEMARK,     ///< Demark marker (no payload)
     COMMS_GUNNAME,    ///< Gun name (32 characters)
     COMMS_size        ///< Total number of commands
 };
 
 /**
  * @brief Payload size lookup table for each CommsCommand.
  *
  * Maps each command in CommsCommand (by enum value) to the size in bytes
  * of its associated payload. For COMMS_MARK (no payload), size is 0.
  */
 static const int payloadSizePerCommand[COMMS_size] = {
     sizeof(int),           ///< COMMS_PLAYERHP
     sizeof(GunData),       ///< COMMS_GUNPARAMS
     sizeof(uint32_t),      ///< COMMS_FIRECODE
     sizeof(GameStatus),    ///< COMMS_GAMESTATUS
     0,                     ///< COMMS_MARK
     0,                     ///< COMMS_DEMARK
     MAX_GUN_NAME_LENGTH    ///< COMMS_GUNNAME
 };
 
 #endif // LAZERTAGPACKET_HPP 