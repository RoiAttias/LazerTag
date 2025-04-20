#include "Game.hpp"

namespace Game {
    Player player1(1);
    Player player2(2);
    GameStatus status = GAME_WAITING;
    NEC_DATA fireSignals[2];

    GameStatus getStatus() {
        return status;
    }

    bool hasPlayerHit(NEC_DATA fireSignal, uint8_t who) {
        if (who == 1) {
            return fireSignal == fireSignals[1];
        } else if (who == 2) {
            return fireSignal == fireSignals[0];
        }
        return false;
    }

    bool processHit(uint8_t id, NEC_DATA fireSignal) {
        if (status != GAME_RUNNING) return false;

        uint8_t who;
        if (id == player1.vestAddress.deviceID) {
            who = 1;
        } else if (id == player2.vestAddress.deviceID) {
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

        byte val = micros() & 0xFF;

        fireSignals[0] = NEC_DATA(val, val + 10);
        fireSignals[1] = NEC_DATA(val + 20, val + 30);
    }

    void run() {
        status = GAME_RUNNING;
    }

    void end() {
        // if (status != GAME_RUNNING) return;
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