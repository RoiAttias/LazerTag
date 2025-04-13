#include "Game.hpp"

namespace Game {
    Player player1(1);
    Player player2(2);
    GameStatus status = GAME_WAITING;
    uint32_t fireSignals[2] = {0, 0};

    GameStatus getStatus() {
        return status;
    }

    bool hasPlayerHit(uint32_t fireSignal, uint8_t who) {
        if (who == 1) {
            return fireSignal == fireSignals[1];
        } else if (who == 2) {
            return fireSignal == fireSignals[0];
        }
        return false;
    }

    bool processHit(uint8_t id, uint32_t fireSignal) {
        if (status != GAME_RUNNING) return false;

        uint8_t who;
        if (id == player1.gunAddress.deviceID) {
            who = 1;
        } else if (id == player2.gunAddress.deviceID) {
            who = 2;
        } else {
            return false; // Invalid player ID
        }

        if (hasPlayerHit(fireSignal, who)) {
            if (who == 1) {
                player1.damage(player2.getGunDamage());
                return true;
            } else if (who == 2) {
                player2.damage(player1.getGunDamage());
                return true;
            }
        }
        return false;
    }

    bool canStart() {
        bool b1 = player1.hasGun() && player2.hasVest();
        bool b2 = player2.hasGun() && player1.hasVest();
        return b1 || b2;
    }

    bool shouldEnd() {
        return player1.getHP() <= 0 || player2.getHP() <= 0;
    }

    void reset() {
        status = GAME_WAITING;

        player1.gunData = GunData(Sidearm);
        player2.gunData = GunData(Sidearm);
    }

    void start() {
        if (status != GAME_WAITING) return;
        status = GAME_STARTING;
        player1.resetHP();
        player2.resetHP();

        do {
            fireSignals[0] = millis() + micros();
            fireSignals[1] = fireSignals[0] << 22 | fireSignals[0] >> 10;
        } while (fireSignals[0] == fireSignals[1]);
    }

    void run() {
        status = GAME_RUNNING;
    }

    void end() {
        if (status != GAME_RUNNING) return;
        status = GAME_OVER;
    }

    uint8_t getWinner() {
        if (player1.getHP() > player2.getHP()) {
            return player1.getID(); // Player 1 wins
        } else if (player2.getHP() > player1.getHP()) {
            return player2.getID(); // Player 2 wins
        } else {
            return 0; // Draw
        }
    }
};