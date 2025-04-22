/**
 * @file Game.cpp
 * @brief Implementation of core game logic: setup, hit detection, state transitions, and result determination.
 */

 #include "Game.hpp"

 namespace Game {
 
     // Initialize two players and default game state
     Player     player1(1);
     Player     player2(2);
     GameStatus status = GAME_WAITING;
     NEC_DATA   fireSignals[2];
 
     GameStatus getStatus() {
         return status;
     }
 
     bool hasPlayerHit(NEC_DATA fireSignal, uint8_t who) {
         // A hit for player1 comes from player2’s code, and vice versa
         if (who == 1) {
             return fireSignal == fireSignals[1];
         } else if (who == 2) {
             return fireSignal == fireSignals[0];
         }
         return false;
     }
 
     bool processHit(uint8_t id, NEC_DATA fireSignal) {
         if (status != GAME_RUNNING) return false;
 
         // Identify which player’s vest was hit
         uint8_t who = 0;
         if (id == player1.getVestAddress().deviceID) {
             who = 1;
         } else if (id == player2.getVestAddress().deviceID) {
             who = 2;
         } else {
             return false; // Unknown target ID
         }
 
         // Apply damage if correct fire code received
         if (hasPlayerHit(fireSignal, who)) {
             if (who == 1) {
                 player1.damage(player2.getGunDamage());
             } else {
                 player2.damage(player1.getGunDamage());
             }
             return true;
         }
         return false;
     }
 
     bool canStart() {
         // Both players must have a gun assigned and the other’s vest reachable
         bool p1Ready = player1.hasGun() && player2.hasVest();
         bool p2Ready = player2.hasGun() && player1.hasVest();
         return p1Ready || p2Ready;
     }
 
     bool shouldEnd() {
         // Game ends when either player’s HP reaches zero
         return player1.getHP() <= 0 || player2.getHP() <= 0;
     }
 
     void reset() {
         status = GAME_WAITING;
         // Restore default weapon for both players
         player1.setGunData(Sidearm);
         player2.setGunData(Sidearm);
     }
 
     void start() {
         if (status != GAME_WAITING) return;
         status = GAME_STARTING;
         player1.resetHP();
         player2.resetHP();
 
         // Generate two unique NEC_DATA codes using micros() low byte
         uint8_t base = micros() & 0xFF;
         fireSignals[0] = NEC_DATA(base, base + 10);
         fireSignals[1] = NEC_DATA(base + 20, base + 30);
     }
 
     void run() {
         status = GAME_RUNNING;
     }
 
     void end() {
         status = GAME_OVER;
     }
 
     uint8_t getWinner() {
         int hp1 = player1.getHP();
         int hp2 = player2.getHP();
         if (hp1 > hp2) return player1.getID();
         if (hp2 > hp1) return player2.getID();
         return 0; // Tie
     }
 
 } // namespace Game 