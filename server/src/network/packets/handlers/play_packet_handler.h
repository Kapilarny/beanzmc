//
// Created by user on 15.07.2024.
//

#ifndef PLAY_PACKET_HANDLER_H
#define PLAY_PACKET_HANDLER_H

#include <boost/container/map.hpp>
#include <vector>


#include "network/packets/data_types.h"

// typedef function
typedef void (*play_packet_handler_t)(const conn::packet &packet);

class play_packet_handler {
public:
    play_packet_handler();

    void handle_packet(const conn::packet &packet);
    void register_handler(u8 packet_id, play_packet_handler_t handler);
private:
    boost::container::map<u8, std::vector<play_packet_handler_t>> registered_handlers;
};

#endif //PLAY_PACKET_HANDLER_H
