//
// Created by user on 13.07.2024.
//

#ifndef SERVER_LOGIN_PACKETS_H
#define SERVER_LOGIN_PACKETS_H
#include "network/packets/data_types.h"

struct ServerDisconnectPacket {
    conn::string reason;
};

struct ServerLoginSuccessPacket {
    conn::uuid uuid{};
    conn::string username;
};

#endif //SERVER_LOGIN_PACKETS_H
