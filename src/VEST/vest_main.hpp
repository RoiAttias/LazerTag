#include "Target/Target.hpp"
#include "Ring/Ring.hpp"

#include "Modules/Game.hpp"
#include "Common/LazerTagPacket.hpp"
#include "Components/Nexus/Nexus.hpp"

int hp = 100;
GameStatus game_status = GameStatus::GAME_WAITING;

void vest_setup() {
  Ring::init();
  Target::init();

  Nexus::begin(NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUPS, NEXUS_DEVICE_ID));

  // Play the loading animation.
  Ring::load1();
}

void vest_loop() {
  Target::loop();
  Ring::loop();
  Nexus::loop();

  // Send the hit data to the manager.
  if (Target::hasHit() > 0) {
    uint32_t firecode = Target::readHit().data;
    if (game_status == GAME_RUNNING){
      //Nexus::sendToGroup(COMMS_FIRECODE, payloadSizePerCommand[COMMS_FIRECODE], (uint8_t*)&firecode, NEXUS_GROUP_MANAGER);
      Nexus::sendData(COMMS_FIRECODE, payloadSizePerCommand[COMMS_FIRECODE], (uint8_t*)&firecode, NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUP_MANAGER, 0xFF));
    }
  }

  // Process any Nexus packets.
  NexusPacket nexusPacket;
  while (Nexus::readPacket(nexusPacket)) {
    // Variables to store the last health and game status.
    // These are used to check if there are any changes.
    // If there are changes, the ring will be updated.
    int lastHP = hp;
    GameStatus prevStatus = game_status;

    // Process the packet based on its command.
    switch (nexusPacket.command)
    {
      case COMMS_PLAYERHP:
        // Update the player's health.
        memcpy(&hp, nexusPacket.payload, payloadSizePerCommand[COMMS_PLAYERHP]);
        if (lastHP > hp) {
          // If the health has decreased, play the hit animation and update the ring.
          Ring::hit(hp);
        }
        break;
      
      case COMMS_GAMESTATUS:
        // Update the game status.
        memcpy(&game_status, nexusPacket.payload, payloadSizePerCommand[COMMS_GAMESTATUS]);
        // If the game status has changed, update the ring.
        if (prevStatus != game_status) {
          switch (game_status)
            {
              case GAME_WAITING:
                // Load the waiting animation.
                Ring::load1();
                break;
              case GAME_STARTING:
                // Load the starting animation.
                Ring::load2();
                break;
              case GAME_THREE:
                Ring::countdown(3);
                break;
              case GAME_TWO:
                Ring::countdown(2);
                break;
              case GAME_ONE:
                Ring::countdown(1);
                break;
              case GAME_GO:
                Ring::countdown(0);
                Target::clear();
                break;
              case GAME_RUNNING:
                Ring::onGameStart(hp);
                break;
              case GAME_OVER:
                Ring::over();
                break;
              
              case GAME_WON:
                Ring::win();
                break;

              case GAME_LOST:
                Ring::lose();
                break;
            }
        }
        break;
      case COMMS_MARK:
        // Play the mark animation.
        Ring::mark();
        break;
    }
  }
}