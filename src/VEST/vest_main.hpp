#include "Target/Target.hpp"
#include "Ring/Ring.hpp"

#include "Modules/Game.hpp"
#include "Common/LazerTagPacket.hpp"
#include "Components/Nexus/Nexus.hpp"

int hp = 100;
GameStatus gameStatus = GameStatus::GAME_WAITING;

void vest_setup() {
  Serial.begin(115200);
  Target::init();
  Ring::init();

  Nexus::begin(NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUPS, NEXUS_DEVICE_ID));

  Ring::load1();
  Serial.println("VEST Setup");
}

void vest_loop() {
  Target::loop();
  Ring::loop();
  Nexus::loop();

  // Send the hit data to the manager.
  if (Target::hasHit() > 0) {
    uint32_t firecode =  Target::readHit().data;
    if (gameStatus == GAME_RUNNING){
      Nexus::sendToGroup(COMMS_FIRECODE, payloadSizePerCommand[COMMS_FIRECODE], (uint8_t*)&firecode, NEXUS_GROUP_MANAGER);
    }
  }

  // Process any Nexus packets.
  NexusPacket nexusPacket;
  while (Nexus::readPacket(nexusPacket)) {
    // Variables to store the last health and game status.
    // These are used to check if there are any changes.
    // If there are changes, the ring will be updated.
    int lastHP = hp;
    GameStatus prevStatus = gameStatus;

    // Process the packet based on its command.
    switch (nexusPacket.command)
    {
      case COMMS_PLAYERHP:
        // Update the player's health.
        memcpy(&hp, nexusPacket.payload, payloadSizePerCommand[COMMS_PLAYERHP]);
        if (lastHP > hp) {
          Ring::hit(hp);
        }
        break;
      
      case COMMS_GAMESTATUS:
        // Update the game status.
        memcpy(&gameStatus, nexusPacket.payload, payloadSizePerCommand[COMMS_GAMESTATUS]);
        if (prevStatus != gameStatus) {
          switch (gameStatus)
            {
              case GAME_WAITING:
                Ring::load1();
                break;
              case GAME_STARTING:
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
                break;
              case GAME_RUNNING:
                Ring::onGameStart();
                break;
              case GAME_OVER:
                Ring::load1();
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