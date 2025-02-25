#ifndef COMMS_HPP
#define COMMS_HPP

#include "Components/Nexus/Nexus.hpp"

enum CommsCommand {
    COMMS_TYPE
    COMMS_GUN_PARAMS,
    COMMS_GUN_FIRECODE,
    
};

struct LazerTagPacket {
    // Header
    uint8_t version[3]; // 3 bytes
    uint32_t command; // 4 bytes

    // Header size = 7 bytes

    uint8_t payload[NEXUS_MAX_PAYLOAD_SIZE - 7]; // 236 bytes
}

#endif // COMMS_HPP