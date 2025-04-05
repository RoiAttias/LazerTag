#ifndef LAZERTAGPACKET_HPP
#define LAZERTAGPACKET_HPP

#include "Modules/Game.hpp"
#include "Modules/Player.hpp"
#include "Modules/Gun.hpp"
#include "Components/Nexus/Nexus.hpp"

#include "Constants_Common.h"

enum CommsCommand : uint32_t {
    COMMS_PLAYERHP,
    COMMS_GUNPARAMS,
    COMMS_FIRECODE,
    COMMS_GAMESTATUS,
    COMMS_MARK,
    COMMS_size
};

const int payloadSizePerCommand[] = {
    sizeof(int),
    sizeof(GunData),
    sizeof(uint32_t),
    sizeof(GameStatus),
    0
};

#endif // LAZERTAGPACKET_HPP