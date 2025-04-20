#ifndef MANAGER_SHARED_HPP
#define MANAGER_SHARED_HPP

#include "Components/Nexus/Nexus.hpp"
#include "Common/LazerTagPacket.hpp"
#include "Modules/Game.hpp"

void startGame() {
    // Player 1
    Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP],
        (uint8_t *)&Game::player1.hp, Game::player1.getGunAddress());
    Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP],
        (uint8_t *)&Game::player1.hp, Game::player1.getVestAddress());
    Nexus::sendData(COMMS_FIRECODE, payloadSizePerCommand[COMMS_FIRECODE],
        (uint8_t *)&Game::fireSignals[0], Game::player1.getGunAddress());
    Nexus::sendData(COMMS_GUNPARAMS, payloadSizePerCommand[COMMS_GUNPARAMS],
        (uint8_t *)&Game::player1.gunData, Game::player1.getGunAddress());

    // Player 2
    Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP],
        (uint8_t *)&Game::player2.hp, Game::player2.getGunAddress());
    Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP],
        (uint8_t *)&Game::player2.hp, Game::player2.getVestAddress());
    Nexus::sendData(COMMS_FIRECODE, payloadSizePerCommand[COMMS_FIRECODE],
        (uint8_t *)&Game::fireSignals[1], Game::player2.getGunAddress());
    Nexus::sendData(COMMS_GUNPARAMS, payloadSizePerCommand[COMMS_GUNPARAMS],
        (uint8_t *)&Game::player2.gunData, Game::player2.getGunAddress());

    // Run the game
    Game::run();

    // Set the game status to GAMEPLAY
    GUI::selectActivity(GUI_Manager_Activity::GAMEPLAY);
    
}

#endif // MANAGER_SHARED_HPP