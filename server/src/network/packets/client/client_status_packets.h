//
// Created by user on 11.07.2024.
//

#ifndef CLIENT_STATUS_PACKETS_H
#define CLIENT_STATUS_PACKETS_H
#include "network/packets/data_types.h"

struct HandshakePacket {
    conn::var_int protocol_version;
    conn::string server_address;
    u16 server_port{};
    conn::var_int next_state;
};

struct RequestPacket {}; // No fields
struct PingPacket {
    i64 payload{};
};

#endif //CLIENT_STATUS_PACKETS_H
