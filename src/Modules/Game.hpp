#ifndef GAME_HPP
#define GAME_HPP

#include "Utilities/HyperList.hpp"

#include "Player.hpp"

enum GameStatus {
    GAME_WAITING,
    GAME_STARTING,
    GAME_RUNNING,
    GAME_OVER,
    GAME_WON,
    GAME_LOST,
};


class Game {
public:
    HyperList<Player> players;
    GameStatus status;

    Game() : players(), status(GAME_WAITING) {}

    GameStatus getStatus() {
        return status;
    }

    bool canStart() {
        bool gunReady = false;
        bool vestReady = false;
        if (players.size() >= 2) {
            for (size_t i = 0; i < players.size(); i++) {
                if (players[i].hasGun()) {
                    gunReady = true;
                }
                if (players[i].hasVest()) {
                    vestReady = true;
                }
            }
            return gunReady && vestReady;
        }
        return false;
    }

    void addPlayer(Player player) {
        if (status != GAME_WAITING) return;
        players.addend(player);
    }

    bool shouldEnd() {
        // Counts alive players
        uint8_t alivePlayers = 0;
        for (size_t i = 0; i < players.size(); i++) {
            if (players[i].isAlive()) {
                alivePlayers++;
            }
        }
        return alivePlayers < 2;
    }

    void reset() {
        players.clear();
        status = GAME_WAITING;
    }

    void start() {
        if (status != GAME_WAITING) return;
        status = GAME_STARTING;
    }

    void end() {
        if (status != GAME_RUNNING) return;
        status = GAME_OVER;
    }

    
};

#endif // GAME_HPP