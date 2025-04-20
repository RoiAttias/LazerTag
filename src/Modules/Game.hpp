#ifndef GAME_HPP
#define GAME_HPP

#include "Utilities/HyperList.hpp"
#include "Components/IRremoteESP32/IRremoteESP32.hpp"

#include "Player.hpp"

enum GameStatus {
    GAME_WAITING,
    GAME_STARTING,
    GAME_THREE,
    GAME_TWO,
    GAME_ONE,
    GAME_GO,
    GAME_RUNNING,
    GAME_OVER,
    GAME_WON,
    GAME_LOST
};

// Decelarations
namespace Game {
    extern Player player1;
    extern Player player2;
    extern GameStatus status;
    extern NEC_DATA fireSignals[2];

    GameStatus getStatus();
    bool hasPlayerHit(NEC_DATA fireSignal, uint8_t who);
    bool processHit(uint8_t id, NEC_DATA fireSignal);
    bool canStart();
    bool shouldEnd();
    void reset();
    void start();
    void run();
    void end();
    uint8_t getWinner();
}

#endif // GAME_HPP