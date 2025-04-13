#ifndef READYSETGO_HPP
#define READYSETGO_HPP

#include "GUI_Manager.hpp"
#include "Utilities/Countdowner.hpp"

void countdownHandler(int parameter);
void readySetGoHandler(ivec2 point, TouchStatus status);

Message *readySetGoMessage = new Message(
    "Are you ready?",
    "",
    "YES",
    readySetGoHandler,
    TFT_NAVY,
    TFT_YELLOW,
    TFT_BLACK
);

String readySetGoText[] = {
    "3",
    "2",
    "1",
    "GO!"
};

void countdownHandler(int parameter) {
    if (parameter < 4) {
        readySetGoMessage->setMessage(readySetGoText[parameter]);
        readySetGoMessage->setButtonVisible(false);
        GUI::callRender();
        countdowner->addEvent(1000, countdownHandler, parameter + 1);

        switch (parameter) {
            case 0:
                Game::status = GameStatus::GAME_THREE;
                break;
            case 1:
                Game::status = GameStatus::GAME_TWO;
                break;
            case 2:
                Game::status = GameStatus::GAME_ONE;
                break;
            case 3:
                Game::status = GameStatus::GAME_GO;
                break;
        }
        // Send the game status to all devices
        Nexus::sendData(COMMS_GAMESTATUS, payloadSizePerCommand[COMMS_GAMESTATUS],
            (uint8_t *)&Game::status, NexusAddress(NEXUS_PROJECT_ID, 0xff, 0xff));
        
    } else if (parameter == 4) {
        // Player 1
        Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP],
            (uint8_t *)&Game::player1.hp, Game::player1.getGunAddress());
        Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP],
            (uint8_t *)&Game::player1.hp, Game::player1.getVestAddress());
        Nexus::sendData(COMMS_FIRECODE, payloadSizePerCommand[COMMS_FIRECODE],
            (uint8_t *)&Game::player1.gunData.damage, Game::player1.getGunAddress());
        Nexus::sendData(COMMS_GUNPARAMS, payloadSizePerCommand[COMMS_GUNPARAMS],
            (uint8_t *)&Game::player1.gunData, Game::player1.getGunAddress());

        // Player 2
        Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP],
            (uint8_t *)&Game::player2.hp, Game::player2.getGunAddress());
        Nexus::sendData(COMMS_PLAYERHP, payloadSizePerCommand[COMMS_PLAYERHP],
            (uint8_t *)&Game::player2.hp, Game::player2.getVestAddress());
        Nexus::sendData(COMMS_FIRECODE, payloadSizePerCommand[COMMS_FIRECODE],
            (uint8_t *)&Game::player2.gunData.damage, Game::player2.getGunAddress());
        Nexus::sendData(COMMS_GUNPARAMS, payloadSizePerCommand[COMMS_GUNPARAMS],
            (uint8_t *)&Game::player2.gunData, Game::player2.getGunAddress());

        // Run the game
        GUI::selectActivity(GUI_Manager_Activity::GAMEPLAY);
        Game::run();
        Serial.println("Game running");
        Nexus::sendData(COMMS_GAMESTATUS, payloadSizePerCommand[COMMS_GAMESTATUS],
            (uint8_t *)&Game::status, NexusAddress(NEXUS_PROJECT_ID, 0xff, 0xff));
    }
}

void readySetGoHandler(ivec2 point, TouchStatus status) {
    if (status == TouchStatus_RELEASE) {
        countdowner->addEvent(100, countdownHandler, 0);
        readySetGoMessage->okButton.OnTouch_setEnable(false);
        status = TouchStatus_READY;
    }
    else if (status == TouchStatus_PRESS) {
        readySetGoMessage->okButton.background.fillColor = TFT_MAROON;
        readySetGoMessage->okButton.background.borderColor = TFT_YELLOW;
        readySetGoMessage->okButton.text.textColor = TFT_YELLOW;
        GUI::callRender();
    }
    
    if (status == TouchStatus_READY) {
        readySetGoMessage->okButton.background.fillColor = TFT_YELLOW;
        readySetGoMessage->okButton.background.borderColor = TFT_BLACK;
        readySetGoMessage->okButton.text.textColor = TFT_BLACK;
        GUI::callRender();
    }
}

#endif // READYSETGO_HPP