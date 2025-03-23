#ifndef LAZERTAGPACKET_HPP
#define LAZERTAGPACKET_HPP

#include "Modules/Game.hpp"
#include "Modules/Player.hpp"
#include "Modules/Gun.hpp"
#include "Components/Nexus/Nexus.hpp"

#include "Constants/Constants_Common.h"

enum CommsCommand : uint32_t {
    COMMS_PLAYERHP,
    COMMS_GUNPARAMS,
    COMMS_FIRECODE,
    COMMS_GAMESTATUS
};


size_t payloadSizePerCommand[] = {
    sizeof(int),
    sizeof(GunData),
    sizeof(uint32_t),
    sizeof(GameStatus)
};

struct __attribute__((packed)) LazerTagPacket {
    // Header
    uint32_t command; // 4 bytes

    // Header size = 4 bytes

    // Payload
    uint8_t payload[NEXUS_MAX_PAYLOAD_SIZE - 4]; // 234 bytes
    // Payload size = 234 bytes = 250 - 12 - 4

    LazerTagPacket(const uint8_t *version, uint32_t command, const uint8_t *payload)
        : command(command) {
        memccpy(this->version, version, 3);
        if (payload != nullptr) {
            memcpy(this->payload, payload, payloadSizePerCommand[command]);
        }
    }

    size_t size() const {
        return 7 + payloadSizePerCommand[command];
    }
};

#endif // LAZERTAGPACKET_HPP