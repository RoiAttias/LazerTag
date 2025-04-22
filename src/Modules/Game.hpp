/**
 * @file Game.hpp
 * @brief Declarations for core game logic, including player management and game state transitions.
 */

 #ifndef GAME_HPP
 #define GAME_HPP
 
 #include "Utilities/HyperList.hpp"           ///< Dynamic list container
 #include "Components/IRremoteESP32/IRremoteESP32.hpp" ///< NEC_DATA for fire signals
 #include "Player.hpp"                         ///< Player class and GunData
 
 /**
  * @enum GameStatus
  * @brief Represents the different phases of the laser-tag game.
  */
 enum GameStatus {
     GAME_WAITING,  ///< Initial idle state, waiting for setup
     GAME_STARTING, ///< Pre-game countdown begins
     GAME_THREE,    ///< Countdown: 3 seconds remaining
     GAME_TWO,      ///< Countdown: 2 seconds remaining
     GAME_ONE,      ///< Countdown: 1 second remaining
     GAME_GO,       ///< 'Go!' signal issued
     GAME_RUNNING,  ///< Active play state
     GAME_OVER,     ///< Play ended, waiting for result
     GAME_WON,      ///< Victory state for a player
     GAME_LOST      ///< Defeat state for a player
 };
 
 /**
  * @namespace Game
  * @brief Encapsulates global game variables and functions.
  */
 namespace Game {
     extern Player     player1;        ///< Player 1 instance
     extern Player     player2;        ///< Player 2 instance
     extern GameStatus status;         ///< Current game status
     extern NEC_DATA   fireSignals[2]; ///< Unique fire codes for each player
 
     /** @brief Get the current game status. */
     GameStatus getStatus();
 
     /**
      * @brief Determine if a received fireSignal matches the opponent’s expected code.
      * @param fireSignal The NEC_DATA received
      * @param who        Player number (1 or 2) we are testing against
      * @return True if signal indicates a hit by the other player
      */
     bool hasPlayerHit(NEC_DATA fireSignal, uint8_t who);
 
     /**
      * @brief Process an incoming hit signal and apply damage if valid.
      * @param id         DeviceID from which the signal originated
      * @param fireSignal NEC_DATA payload of the signal
      * @return True if hit was processed and damage applied
      */
     bool processHit(uint8_t id, NEC_DATA fireSignal);
 
     /** @brief Check if both players have valid equipment to start the game. */
     bool canStart();
 
     /** @brief Check if the game should end (a player’s HP reached zero). */
     bool shouldEnd();
 
     /** @brief Reset game status and reload default weapon loadouts. */
     void reset();
 
     /**
      * @brief Begin the pre-game sequence:
      *  - Transition status to GAME_STARTING
      *  - Reset HP for both players
      *  - Generate two distinct NEC fireSignals for each player
      */
     void start();
 
     /** @brief Enter active play state (GAME_RUNNING). */
     void run();
 
     /** @brief End the game and set status to GAME_OVER. */
     void end();
 
     /**
      * @brief Determine the winner based on remaining HP.
      * @return Player ID of the winner, or 0 on draw
      */
     uint8_t getWinner();
 }
 
 #endif // GAME_HPP