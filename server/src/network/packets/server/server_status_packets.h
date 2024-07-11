//
// Created by user on 11.07.2024.
//

#ifndef SERVER_STATUS_PACKETS_H
#define SERVER_STATUS_PACKETS_H

#include "network/packets/data_types.h"

struct StatusResponsePacket {
    conn::string json_response;
};

#endif //SERVER_STATUS_PACKETS_H