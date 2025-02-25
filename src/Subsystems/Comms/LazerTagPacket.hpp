#ifndef LAZERTAGPACKET_HPP
#define LAZERTAGPACKET_HPP

#include "Modules/Player.hpp"
#include "Modules/Gun.hpp"
#include "Modules/Team.hpp"

#include "Constants/Constants_Common.h"

enum CommsCommand : uint32_t {
    COMMS_TYPE
    COMMS_GUNPARAMS,
    COMMS_FIRECODE,
    COMMS_HP,
    COMMS_TEAM,
    COMMS_COLOR,
};

uint8_t payloadSizePerCommand[] = {
    1, // COMMS_TYPE
    sizeof(GunData), // COMMS_GUNPARAMS
    4, // COMMS_FIRECODE
    sizeof(int), // COMMS_HP
    1, // COMMS_TEAM
    3, // COMMS_COLOR
};

struct __attribute__((packed)) LazerTagPacket {
    // Header
    uint8_t version[3]; // 3 bytes
    uint32_t command; // 4 bytes

    // Header size = 7 bytes

    uint8_t payload[NEXUS_MAX_PAYLOAD_SIZE - 7]; // 236 bytes

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