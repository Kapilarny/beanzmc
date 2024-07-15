//
// Created by user on 15.07.2024.
//

#include "play_packet_handler.h"

#include "impl/client_keep_alive.h"

play_packet_handler::play_packet_handler() {
    register_handler(handle::KEEP_ALIVE_PACKET_ID, handle::handle_keep_alive);
}

void play_packet_handler::handle_packet(const conn::packet &packet) {
    for(auto& handler : registered_handlers[packet.packetID.val]) {
        handler(packet);
    }
}

void play_packet_handler::register_handler(u8 packet_id, play_packet_handler_t handler) {
    registered_handlers[packet_id].push_back(handler);
}
