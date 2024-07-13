//
// Created by user on 10.07.2024.
//

#include "server.h"

#include <iostream>

#include "packets/data_types.h"
#include "packets/packet_serializer.h"

void tcp_connection::start() {
    std::cout << "New connection created!\n";

    try {
        // Read data
        while (socket_.is_open() && socket_.available() != 0) {
            conn::packet p{};

            // Read packet length
            p.length.val = read_varint_from_socket();

            std::cout << "Received length: " << p.length.val << "\n";

            if (p.length.val > 0) {
                // Read packet ID
                p.packetID.val = read_varint_from_socket();

                std::cout << "Received packet ID: " << p.packetID.val << "\n";

                // Read packet data
                p.data = new u8[p.length.val];
                boost::asio::read(socket_, boost::asio::buffer(p.data, p.length.val - get_varint_size(p.packetID.val))); // TODO: make it more efficient
            }

            dispatcher.dispatch_packet(p);

            delete[] p.data;
        }
    } catch (boost::system::system_error &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << "Connection TERMINATED!\n";
}

void tcp_connection::write_packet(conn::packet packet) {
    // Write packet length
    std::vector<u8> length = var_int_gen_arr(packet.length.val);
    boost::asio::write(socket_, boost::asio::buffer(length));

    // Write packet ID
    std::vector<u8> packet_id = var_int_gen_arr(packet.packetID.val);
    boost::asio::write(socket_, boost::asio::buffer(packet_id));

    // Write packet data
    if (packet.length.val > 0) {
        boost::asio::write(socket_, boost::asio::buffer(packet.data, packet.length.val - packet_id.size()));
    }

    // Print packet data
    std::cout << "PACKET DISPATCHED! (" << packet.length.val << ", " << packet.packetID.val << ")\n\n";

    for(int i = 0; i < length.size(); i++) {
        std::cout << std::hex << (int)length[i] << " ";
    }

    for(int i = 0; i < packet_id.size(); i++) {
        std::cout << std::hex << (int)packet_id[i] << " ";
    }

    for(int i = 0; i < packet.length.val - packet_id.size(); i++) {
        std::cout << std::hex << (int)packet.data[i] << " ";
    }

    std::cout << "\n\n\n";
}

i32 tcp_connection::read_varint_from_socket() {
    i32 value = 0;
    i32 shift = 0;

    while (true) {
        u8 byte;
        if(!boost::asio::read(socket_, boost::asio::buffer((void*)&byte, 1))) {
            throw std::runtime_error("Failed to read packet length");
        }

        value |= (byte & SEGMENT_BITS) << shift;

        if((byte & CONTINUE_BIT) == 0) break;
        shift += 7;

        if(shift >= 32) throw std::runtime_error("VarInt is too big");
    }

    return value;
}

void tcp_server::start_accept() {
    tcp_connection::pointer new_connection =
        tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code &error) {
    if(!error) {
        new_connection->start();
    }

    start_accept();
}
