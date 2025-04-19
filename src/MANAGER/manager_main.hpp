#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
#include "Common/Constants_Common.h"
#include "Common/LazerTagPacket.hpp"
#include "Components/Nexus/Nexus.hpp"
#include "Modules/Game.hpp"
#include "GUI_Manager/GUI_Manager.hpp"

void scanCompletedCallback()
{
    scanner->updateScannedDevices();
    GUI::callRender();
}


void manager_setup()
{
    Serial.begin(115200);
    Nexus::begin(NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUPS, NEXUS_DEVICE_ID));
    Nexus::onScanComplete = scanCompletedCallback;
    GUI::init(GUI_Manager_Activity::ACTIVATION);

    Game::reset();
}

void manager_loop()
{   
    Nexus::loop();
    GUI::loop();
    countdowner->loop();

    if (Game::status == GAME_RUNNING)
    {
        NexusPacket nexusPacket;
        while (Nexus::readPacket(nexusPacket)) {
            Serial.println(nexusPacket.toString());
            if (nexusPacket.command == COMMS_FIRECODE && nexusPacket.source.groups == NEXUS_GROUP_VEST)
            {
                // Process the fire signal from the vest
                uint32_t fireSignal;
                memcpy((uint8_t *)&fireSignal, nexusPacket.payload, payloadSizePerCommand[COMMS_FIRECODE]);
                if (Game::processHit(nexusPacket.source.deviceID, fireSignal))
                {
                    // Send the updated HP to the gun and vest                    
                    Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP], (uint8_t *)&Game::player1.hp, Game::player1.getGunAddress());
                    Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP], (uint8_t *)&Game::player1.hp, Game::player1.getVestAddress());
                    Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP], (uint8_t *)&Game::player2.hp, Game::player2.getGunAddress());
                    Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP], (uint8_t *)&Game::player2.hp, Game::player2.getVestAddress());
                    
                    GUI::callRender();
                }
            }
        }

        if (Game::shouldEnd())
        {
            Game::end();
            GUI::callRender();
            Nexus::sendData(COMMS_GAMESTATUS, payloadSizePerCommand[COMMS_GAMESTATUS],
                (uint8_t *)&Game::status, NexusAddress(NEXUS_PROJECT_ID, 0xff, 0xff));
            
            GUI::loop();

            delay(3000);

            uint8_t winner = Game::getWinner();
            GameStatus won = GameStatus::GAME_WON, lost = GameStatus::GAME_LOST;
            if (Game::player1.getID() == winner)
            {
                Nexus::sendData(COMMS_GAMESTATUS, payloadSizePerCommand[COMMS_GAMESTATUS], (uint8_t *)&won, Game::player1.getGunAddress());
                Nexus::sendData(COMMS_GAMESTATUS, payloadSizePerCommand[COMMS_GAMESTATUS], (uint8_t *)&lost, Game::player2.getGunAddress());
                
                Nexus::sendData(COMMS_MARK, payloadSizePerCommand[COMMS_MARK], nullptr, Game::player1.getGunAddress());
                Nexus::sendData(COMMS_MARK, payloadSizePerCommand[COMMS_MARK], nullptr, Game::player1.getVestAddress());
            } 
            else if (Game::player2.getID() == winner)
            {
                Nexus::sendData(COMMS_GAMESTATUS, payloadSizePerCommand[COMMS_GAMESTATUS], (uint8_t *)&lost, Game::player1.getGunAddress());
                Nexus::sendData(COMMS_GAMESTATUS, payloadSizePerCommand[COMMS_GAMESTATUS], (uint8_t *)&won, Game::player2.getGunAddress());
            
                Nexus::sendData(COMMS_MARK, payloadSizePerCommand[COMMS_MARK], nullptr, Game::player2.getGunAddress());
                Nexus::sendData(COMMS_MARK, payloadSizePerCommand[COMMS_MARK], nullptr, Game::player2.getVestAddress());
            }
        }
    }
}

#endif // MANAGER_MAIN_HPP