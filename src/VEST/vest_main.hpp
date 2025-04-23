/**
 * @file vest_main.cpp
 * @brief Main application code for the Vest device in the LaserTag system.
 *
 * Initializes subsystems (Ring, Target), connects to the Nexus network,
 * and in the main loop processes IR hits, game state updates, and
 * triggers the appropriate LED animations on the NeoPixel ring.
 */

 #include "Target/Target.hpp"              ///< Target subsystem for hit detection
 #include "Ring/Ring.hpp"                  ///< Ring subsystem for LED animations
 #include "Modules/Game.hpp"               ///< Game logic (status, hit processing)
 #include "Common/LazerTagPacket.hpp"      ///< Communication packet definitions
 #include "Components/Nexus/Nexus.hpp"     ///< ESP-NOW networking
 
 int hp = 100;                             ///< Local copy of current health
 GameStatus game_status = GAME_WAITING;    ///< Local copy of current game status
 
 /**
  * @brief Called once at startup.
  *
  * - Initializes the Ring and Target subsystems.
  * - Connects to the Nexus network using project/group/device IDs.
  * - Starts the first loading animation on the ring.
  */
 void vest_setup() {
   Ring::init();
   Target::init();
 
   // Begin ESP-NOW in broadcast mode
   Nexus::begin(NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUPS, NEXUS_DEVICE_ID));
 
   // Play the first loading animation
   Ring::load1();
 }
 
 /**
  * @brief Called continuously in the Arduino loop.
  *
  * - Polls the Target subsystem for new IR hits.
  * - Updates Ring animations.
  * - Processes incoming Nexus packets to update health and game state.
  * - Sends hit events to the Manager when appropriate.
  */
 void vest_loop() {
   // Update subsystems
   Target::loop();
   Ring::loop();
   Nexus::loop();
 
   // If the vest has registered a hit, forward it to the manager
   if (Target::hasHit() > 0) {
     uint32_t firecode = Target::readHit().data;
     if (game_status == GAME_RUNNING) {
       // Send the firecode to all managers (broadcast to manager group)
       Nexus::sendData(
         COMMS_FIRECODE,
         payloadSizePerCommand[COMMS_FIRECODE],
         (uint8_t*)&firecode,
         NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUP_MANAGER, 0xFF)
       );
     }
   }
 
   // Process all pending Nexus packets
   NexusPacket packet;
   while (Nexus::readPacket(packet)) {
     int lastHP = hp;
     GameStatus prevStatus = game_status;
 
     switch (packet.command) {
       case COMMS_PLAYERHP:
         // Update HP from payload
         memcpy(&hp, packet.payload, payloadSizePerCommand[COMMS_PLAYERHP]);
         if (lastHP > hp) {
           // Health decreased → play hit animation
           Ring::hit(hp);
         }
         break;
 
       case COMMS_GAMESTATUS:
         // Update game status from payload
         memcpy(&game_status, packet.payload, payloadSizePerCommand[COMMS_GAMESTATUS]);
         if (prevStatus != game_status) {
           // On status change, trigger the corresponding Ring animation
           switch (game_status) {
             case GAME_WAITING:  Ring::load1();                       break;
             case GAME_STARTING: Ring::load2();                       break;
             case GAME_THREE:    Ring::countdown(3);                  break;
             case GAME_TWO:      Ring::countdown(2);                  break;
             case GAME_ONE:      Ring::countdown(1);                  break;
             case GAME_GO:       Ring::countdown(0); Target::clear(); break;
             case GAME_RUNNING:  Ring::onGameStart(hp);               break;
             case GAME_OVER:     Ring::over();                        break;
             case GAME_WON:      Ring::win();                         break;
             case GAME_LOST:     Ring::lose();                        break;
             default:                                           break;
           }
         }
         break;
 
       case COMMS_MARK:
         // Play a brief "mark" animation (e.g. yellow flash)
         Ring::mark();
         break;
 
       default:
         // Unknown command: ignore
         break;
     }
   }
 }
 