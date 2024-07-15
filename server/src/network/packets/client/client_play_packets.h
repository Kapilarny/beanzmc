//
// Created by user on 15.07.2024.
//

#ifndef CLIENT_PLAY_PACKETS_H
#define CLIENT_PLAY_PACKETS_H

#include "network/packets/data_types.h"

struct ClientTeleportConfirmPacket {
    conn::var_int teleport_id;
};

struct ClientKeepAlivePacket {
    i64 id{};
};

#endif //CLIENT_PLAY_PACKETS_H
