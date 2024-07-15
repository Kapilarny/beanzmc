//
// Created by user on 15.07.2024.
//

#ifndef CLIENT_KEEP_ALIVE_H
#define CLIENT_KEEP_ALIVE_H

#include "network/packets/client/client_play_packets.h"
#include "network/packets/handlers/play_packet_handler.h"
#include "network/packets/packet_serializer.h"

namespace handle {
    inline void handle_keep_alive(const conn::packet& packet) {
        auto keep_alive_packet = read_packet_data<ClientKeepAlivePacket>(packet);
        std::cout << "Keep alive packet received: " << keep_alive_packet.id << std::endl;
    }

    constexpr u8 KEEP_ALIVE_PACKET_ID = 0x10;
}

#endif //CLIENT_KEEP_ALIVE_H
