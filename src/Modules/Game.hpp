#ifndef GAME_HPP
#define GAME_HPP

#include "Utilities/HyperList.hpp"

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
    extern uint32_t fireSignals[2];

    GameStatus getStatus();
    bool hasPlayerHit(uint32_t fireSignal, uint8_t who);
    bool processHit(uint8_t id, uint32_t fireSignal);
    bool canStart();
    bool shouldEnd();
    void reset();
    void start();
    void run();
    void end();
    uint8_t getWinner();
}

#endif // GAME_HPP