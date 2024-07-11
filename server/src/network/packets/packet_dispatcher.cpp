//
// Created by user on 11.07.2024.
//

#include "packet_dispatcher.h"

#include <iostream>

#include "client/client_status_packets.h"
#include "network/server.h"
#include "packet_serializer.h"
#include "server/server_status_packets.h"

packet_dispatcher::packet_dispatcher(tcp_connection &connection) : connection(connection) {}

void packet_dispatcher::dispatch_packet(const conn::packet &packet) {
    std::cout << "Dispatching packet with ID: " << packet.packetID.val << std::endl;

    // Handle packet
    switch (state) {
        case conn::ConnectionState::HANDSHAKE:
            handle_handshake(packet);
            break;
        default:
            std::cout << "Unknown connection state" << std::endl;
            break;
    }
}

void packet_dispatcher::handle_handshake(const conn::packet &packet) {
    if(packet.packetID.val != conn::StatusPacketType::HANDSHAKE) {
        std::cout << "Invalid packet ID" << std::endl;
        return;
    }

    // Switch to the next state
    auto handshake_packet = read_packet_data<HandshakePacket>(packet);

    std::cout << "Handshake packet received" << std::endl;

    std::cout << "Switching to state -> " << handshake_packet.next_state.val << std::endl;
    state = (conn::ConnectionState) handshake_packet.next_state.val;

    if(state == conn::ConnectionState::STATUS) {
        // Wait for a ping
        connection.read_varint_from_socket();

        // Send status response packet
        StatusResponsePacket response_packet{};
        std::string json_data = R"(
        {
            "version": {
                "name": "1.16.5",
                "protocol": 754
            },
            "players": {
                "max": 100,
                "online": 5,
                "sample": [
                    {
                        "name": "thinkofdeath",
                        "id": "4566e69f-c907-48ee-8d71-d7ba5aa00d20"
                    }
                ]
            },
            "description": {
                "text": "Hello, world!"
            }
        }
        )";

        // Set JSON data
        response_packet.json_response = string_gen(json_data);

        // Serialize packet
        conn::packet p = write_packet_data(response_packet, 0x00);

        // Send packet
        connection.write_packet(p);

        std::cout << "Written JSON response!\n";
    }
}
