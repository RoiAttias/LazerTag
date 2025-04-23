/**
 * @file manager_main.hpp
 * @brief Main firmware for the Manager device in the LaserTag system.
 *
 * Responsibilities:
 * - Initialize Nexus (ESP-NOW) networking and GUI.
 * - Scan for Vest/Gun devices and display them.
 * - Receive fire signals from Vests, process hits, and update HP.
 * - Detect end-of-game and schedule Winner/Loser notifications.
 */

 #ifndef MANAGER_MAIN_HPP
 #define MANAGER_MAIN_HPP
 
 #include <Arduino.h>
 #include "Common/Constants_Common.h"
 #include "Common/LazerTagPacket.hpp"
 #include "Components/Nexus/Nexus.hpp"
 #include "Utilities/Countdowner.hpp"
 #include "Modules/Game.hpp"
 #include "GUI_Manager/GUI_Manager.hpp"
 
 /**
  * @brief Callback invoked by Countdowner to notify Winner and Loser after a delay.
  * @param parameter The ID of the winning player (1 or 2).
  */
 void countdowner_WinnerLoser(int parameter);
 
 /**
  * @brief Called when Nexus device scan completes.
  *
  * Refreshes the list of scanned devices in the GUI and triggers a redraw.
  */
 void scanCompletedCallback()
 {
     // Update the scanned devices list in the scanner UI component
     scanner->updateScannedDevices();
     // Request GUI redraw
     GUI::callRender();
 }
 
 /**
  * @brief Setup routine for the Manager device.
  *
  * - Starts serial at 115200 bps.
  * - Initializes Nexus ESP-NOW with this device's address.
  * - Registers the scan-complete callback.
  * - Initializes the Manager GUI to the ACTIVATION screen.
  * - Resets the game state to waiting.
  */
 void manager_setup()
 {
     Serial.begin(115200);
 
     // Initialize ESP-NOW networking (Nexus)
     Nexus::begin(NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUPS, NEXUS_DEVICE_ID));
 
     // When scan completes, invoke our callback
     Nexus::onScanComplete = scanCompletedCallback;
 
     // Initialize GUI to the "Activation" activity (device selection)
     GUI::init(GUI_Manager_Activity::ACTIVATION);
 
     // Reset game data (HP, fire codes, status)
     Game::reset();
 }
 
 /**
  * @brief Main loop for the Manager device.
  *
  * - Processes Nexus networking events.
  * - Updates GUI and Countdowner timers.
  * - Reads incoming NexusPackets:
  *   • During GAME_RUNNING: handles COMMS_FIRECODE from Vests, processes hits,
  *     and broadcasts updated HP to all devices.
  *   • When game-end condition met: ends game, broadcasts GAMESTATUS, and schedules
  *     delayed Winner/Loser notifications.
  */
 void manager_loop()
 {
     // Handle any pending Nexus callbacks or sends
     Nexus::loop();
 
     // Update GUI rendering loop
     GUI::loop();
 
     // Process scheduled events (e.g., delayed Winner/Loser)
     countdowner->loop();
 
     NexusPacket packet;
     // Consume all available received packets
     while (Nexus::readPacket(packet)) {
         // Only process hits during active gameplay
         if (Game::status == GAME_RUNNING) {
 
             // Handle incoming fire signals from Vest devices
             if (packet.command == COMMS_FIRECODE
                 && packet.source.groups == NEXUS_GROUP_VEST)
             {
                 // Extract NEC code from payload
                 NEC_DATA fireSignal;
                 memcpy(&fireSignal, packet.payload,
                        payloadSizePerCommand[COMMS_FIRECODE]);
 
                 // If the hit is valid, apply damage and broadcast new HP
                 if (Game::processHit(packet.source.deviceID, fireSignal))
                 {
                     // Broadcast updated HP to both players' Gun and Vest
                     Nexus::sendData(
                         COMMS_PLAYERHP,
                         payloadSizePerCommand[COMMS_PLAYERHP],
                         (uint8_t*)&Game::player1.hp,
                         Game::player1.getGunAddress());
                     Nexus::sendData(
                         COMMS_PLAYERHP,
                         payloadSizePerCommand[COMMS_PLAYERHP],
                         (uint8_t*)&Game::player1.hp,
                         Game::player1.getVestAddress());
                     Nexus::sendData(
                         COMMS_PLAYERHP,
                         payloadSizePerCommand[COMMS_PLAYERHP],
                         (uint8_t*)&Game::player2.hp,
                         Game::player2.getGunAddress());
                     Nexus::sendData(
                         COMMS_PLAYERHP,
                         payloadSizePerCommand[COMMS_PLAYERHP],
                         (uint8_t*)&Game::player2.hp,
                         Game::player2.getVestAddress());
 
                     // Trigger GUI update
                     GUI::callRender();
                 }
             }
 
             // If either player has 0 HP, end the game
             if (Game::shouldEnd()) {
                 // Transition game state to GAME_OVER
                 Game::end();
 
                 // Broadcast GAME_OVER to all participants (Gun & Vest)
                 Nexus::sendData(
                     COMMS_GAMESTATUS,
                     payloadSizePerCommand[COMMS_GAMESTATUS],
                     (uint8_t*)&Game::status,
                     NexusAddress(NEXUS_PROJECT_ID, 0xFF, 0xFF));
 
                 // Request GUI update
                 GUI::callRender();
 
                 // Schedule delayed Winner/Loser packet sends after 3000 ms
                 uint8_t winner = Game::getWinner();
                 countdowner->addEvent(
                     3000,
                     countdowner_WinnerLoser,
                     winner);
             }
         }
     }
 }
 
 /**
  * @brief Sends WIN/LOSE status and MARK events to appropriate devices after delay.
  * @param parameter Winning player ID (1 or 2).
  */
 void countdowner_WinnerLoser(int parameter)
 {
     uint8_t winner = (uint8_t)parameter;
     GameStatus won = GAME_WON;
     GameStatus lost = GAME_LOST;
 
     // If Player 1 won
     if (Game::player1.getID() == winner)
     {
         // Notify Gun/Vest of Player1: WON
         Nexus::sendData(
             COMMS_GAMESTATUS,
             payloadSizePerCommand[COMMS_GAMESTATUS],
             (uint8_t*)&won,
             Game::player1.getGunAddress());
         Nexus::sendData(
             COMMS_GAMESTATUS,
             payloadSizePerCommand[COMMS_GAMESTATUS],
             (uint8_t*)&won,
             Game::player1.getVestAddress());
 
         // Notify Gun/Vest of Player2: LOST
         Nexus::sendData(
             COMMS_GAMESTATUS,
             payloadSizePerCommand[COMMS_GAMESTATUS],
             (uint8_t*)&lost,
             Game::player2.getGunAddress());
         Nexus::sendData(
             COMMS_GAMESTATUS,
             payloadSizePerCommand[COMMS_GAMESTATUS],
             (uint8_t*)&lost,
             Game::player2.getVestAddress());
 
         // Flash mark on winner’s endpoints
         Nexus::sendData(
             COMMS_MARK,
             payloadSizePerCommand[COMMS_MARK],
             nullptr,
             Game::player1.getGunAddress());
         Nexus::sendData(
             COMMS_MARK,
             payloadSizePerCommand[COMMS_MARK],
             nullptr,
             Game::player1.getVestAddress());
     }
     // If Player 2 won
     else if (Game::player2.getID() == winner)
     {
         // Notify Player2 WON, Player1 LOST
         Nexus::sendData(
             COMMS_GAMESTATUS,
             payloadSizePerCommand[COMMS_GAMESTATUS],
             (uint8_t*)&won,
             Game::player2.getGunAddress());
         Nexus::sendData(
             COMMS_GAMESTATUS,
             payloadSizePerCommand[COMMS_GAMESTATUS],
             (uint8_t*)&won,
             Game::player2.getVestAddress());
 
         Nexus::sendData(
             COMMS_GAMESTATUS,
             payloadSizePerCommand[COMMS_GAMESTATUS],
             (uint8_t*)&lost,
             Game::player1.getGunAddress());
         Nexus::sendData(
             COMMS_GAMESTATUS,
             payloadSizePerCommand[COMMS_GAMESTATUS],
             (uint8_t*)&lost,
             Game::player1.getVestAddress());
 
         // Flash mark on Player2's endpoints
         Nexus::sendData(
             COMMS_MARK,
             payloadSizePerCommand[COMMS_MARK],
             nullptr,
             Game::player2.getGunAddress());
         Nexus::sendData(
             COMMS_MARK,
             payloadSizePerCommand[COMMS_MARK],
             nullptr,
             Game::player2.getVestAddress());
     }
 }
 
 #endif // MANAGER_MAIN_HPP 