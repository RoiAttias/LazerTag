/**
 * @file manager_shared.hpp
 * @brief Shared helper logic for Manager activities to start a new game session.
 *
 * Provides a single function to:
 * - Broadcast each player’s starting HP and fire codes.
 * - Send initial GunData parameters.
 * - Transition game state to RUNNING and switch GUI to GAMEPLAY.
 */

 #ifndef MANAGER_SHARED_HPP
 #define MANAGER_SHARED_HPP
 
 #include "Components/Nexus/Nexus.hpp"
 #include "Common/LazerTagPacket.hpp"
 #include "Modules/Game.hpp"
 
 /**
  * @brief Broadcasts game start data to both players and switches GUI.
  *
  * Sends to Player1 & Player2 Gun/Vest:
  *  1. COMMS_PLAYERHP  – initial HP
  *  2. COMMS_FIRECODE – unique NEC fire code
  *  3. COMMS_GUNPARAMS – initial GunData
  * Then sets Game::status = GAME_RUNNING and selects the GAMEPLAY activity.
  */
 void startGame() {
     // Player 1 initialization
     Nexus::sendData(
         COMMS_PLAYERHP,
         payloadSizePerCommand[COMMS_PLAYERHP],
         (uint8_t*)&Game::player1.hp,
         Game::player1.getGunAddress());
     Nexus::sendData(
         COMMS_PLAYERHP,
         payloadSizePerCommand[COMMS_PLAYERHP],
         (uint8_t*)&Game::player1.hp,
         Game::player1.getVestAddress());
     Nexus::sendData(
         COMMS_FIRECODE,
         payloadSizePerCommand[COMMS_FIRECODE],
         (uint8_t*)&Game::fireSignals[0],
         Game::player1.getGunAddress());
     Nexus::sendData(
         COMMS_GUNPARAMS,
         payloadSizePerCommand[COMMS_GUNPARAMS],
         (uint8_t*)&Game::player1.gunData,
         Game::player1.getGunAddress());
     Nexus::sendData(
        COMMS_GUNNAME,
        payloadSizePerCommand[COMMS_GUNNAME],
        (uint8_t*)Game::player1.gunName,
        Game::player1.getGunAddress());
 
     // Player 2 initialization
     Nexus::sendData(
         COMMS_PLAYERHP,
         payloadSizePerCommand[COMMS_PLAYERHP],
         (uint8_t*)&Game::player2.hp,
         Game::player2.getGunAddress());
     Nexus::sendData(
         COMMS_PLAYERHP,
         payloadSizePerCommand[COMMS_PLAYERHP],
         (uint8_t*)&Game::player2.hp,
         Game::player2.getVestAddress());
     Nexus::sendData(
         COMMS_FIRECODE,
         payloadSizePerCommand[COMMS_FIRECODE],
         (uint8_t*)&Game::fireSignals[1],
         Game::player2.getGunAddress());
     Nexus::sendData(
         COMMS_GUNPARAMS,
         payloadSizePerCommand[COMMS_GUNPARAMS],
         (uint8_t*)&Game::player2.gunData,
         Game::player2.getGunAddress());
     Nexus::sendData(
        COMMS_GUNNAME,
        payloadSizePerCommand[COMMS_GUNNAME],
        (uint8_t*)Game::player2.gunName,
        Game::player2.getGunAddress());
 
     // Change game state and GUI
     Game::run();
     GUI::selectActivity(GUI_Manager_Activity::GAMEPLAY);
 }
 
 #endif // MANAGER_SHARED_HPP 