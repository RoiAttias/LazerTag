#include "Subsystems/Target/Target.hpp"
#include "Subsystems/Ring/Ring.hpp"

#include "Modules/Game.hpp"
#include "Common/LazerTagPacket.hpp"
#include "Components/Nexus/Nexus.hpp"

int hp = 100;
GameStatus gameStatus = GameStatus::GAME_WAITING;

void vest_setup() {
  Target::init();
  Ring::init();

  Nexus::begin(NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUPS, NEXUS_DEVICE_ID));

  Ring::load1();
}

void vest_loop() {
  Target::loop();
  Ring::loop();
  Nexus::loop();

  // Send the hit data to the manager.
  if (Target::hasHit() > 0) {
    uint32_t firecode =  Target::readHit().data;
    Nexus::sendToGroup(COMMS_FIRECODE, payloadSizePerCommand[COMMS_FIRECODE], (uint8_t*)&firecode, NEXUS_GROUP_MANAGER);
  }

  // Process any Nexus packets.
  NexusPacket nexusPacket;
  while (Nexus::readPacket(nexusPacket)) {
    // Process the packet based on its command.
    switch (nexusPacket.command)
    {
      case COMMS_PLAYERHP:
        // Update the player's health.
        int lastHP = hp;
        memcpy(&hp, nexusPacket.payload, payloadSizePerCommand[COMMS_PLAYERHP]);
        if (lastHP > hp) {
          Ring::hit();
        }
        break;
      case COMMS_GAMESTATUS:
        // Update the game status.
        GameStatus prevStatus = gameStatus;
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
              case GAME_OVER:
                Ring::load1();
                break;
            }
        }
        break;
    }
  }
}