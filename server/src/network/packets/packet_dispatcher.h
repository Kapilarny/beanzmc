//
// Created by user on 11.07.2024.
//

#ifndef PACKET_DISPATCHER_H
#define PACKET_DISPATCHER_H

#include "data_types.h"

class tcp_connection;

class packet_dispatcher {
public:
    explicit packet_dispatcher(tcp_connection &connection);

    void dispatch_packet(const conn::packet& packet);
    void send_packet(const conn::packet& packet);
private:
    void handle_handshake(const conn::packet& packet);

    tcp_connection& connection;
    conn::ConnectionState state = conn::ConnectionState::HANDSHAKE;
};



#endif //PACKET_DISPATCHER_H
