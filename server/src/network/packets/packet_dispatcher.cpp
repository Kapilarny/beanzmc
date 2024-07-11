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
        case conn::ConnectionState::STATUS:
            handle_status(packet);
            break;
        default:
            std::cout << "Unknown connection state" << std::endl;
            break;
    }
}

void packet_dispatcher::handle_handshake(const conn::packet &packet) {
    if (packet.packetID.val != conn::StatusPacketType::HANDSHAKE) {
        std::cout << "Invalid packet ID" << std::endl;
        return;
    }

    // Switch to the next state
    auto handshake_packet = read_packet_data<HandshakePacket>(packet);

    std::cout << "Handshake packet received" << std::endl;

    std::cout << "Switching to state -> " << handshake_packet.next_state.val << std::endl;
    state = (conn::ConnectionState) handshake_packet.next_state.val;
}

void packet_dispatcher::handle_status(const conn::packet &packet) {
    switch (packet.packetID.val) {
        case 0x00: {
            std::cout << "Received status request packet\n";

            // Send status response packet
            StatusResponsePacket response_packet{};
            std::string json_data = "{\n"
                                    "  \"version\": {\n"
                                    "    \"name\": \"1.16.5\",\n"
                                    "    \"protocol\": 754\n"
                                    "  },\n"
                                    "  \"players\": {\n"
                                    "    \"max\": 100,\n"
                                    "    \"online\": 50\n"
                                    "  },\n"
                                    "  \"description\": {\n"
                                    "    \"text\": \"Hello, world!\"\n"
                                    "  }\n"
                                    "}";

            // Set JSON data
            response_packet.json_response = string_gen(json_data);

            // Serialize packet
            conn::packet p = write_packet_data(response_packet, 0x00);

            // Send packet
            connection.write_packet(p);

            std::cout << "Written JSON response!\n";
        } break;
        default:
            std::cout << "Unknown packet ID" << std::endl;
            break;
    }
}
