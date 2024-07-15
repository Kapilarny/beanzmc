//
// Created by user on 11.07.2024.
//

#ifndef PACKET_DISPATCHER_H
#define PACKET_DISPATCHER_H

#include <boost/uuid/random_generator.hpp>


#include "data_types.h"
#include "handlers/play_packet_handler.h"
#include "network/mc_state_handler.h"

class tcp_connection;

class packet_dispatcher {
public:
    explicit packet_dispatcher(tcp_connection &connection);

    void dispatch_packet(const conn::packet& packet);
private:
    void handle_handshake(const conn::packet& packet);
    void handle_status(const conn::packet& packet);
    void handle_login(const conn::packet& packet);

    tcp_connection& connection;
    mc_state_handler state_handler;
    play_packet_handler play_packet_handler{};
    conn::ConnectionState state = conn::ConnectionState::HANDSHAKE;
    boost::uuids::random_generator uuid_generator = boost::uuids::random_generator();
};



#endif //PACKET_DISPATCHER_H
