//
// Created by user on 13.07.2024.
//

#ifndef CLIENT_LOGIN_PACKETS_H
#define CLIENT_LOGIN_PACKETS_H

#include "network/packets/data_types.h"

struct ClientLoginStartPacket {
    conn::string name;
};

#endif //CLIENT_LOGIN_PACKETS_H
