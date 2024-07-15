//
// Created by user on 10.07.2024.
//

#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "defines.h"
#include "packets/data_types.h"
#include "packets/packet_dispatcher.h"

using boost::asio::ip::tcp;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context) {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket &socket() { return socket_; }

    void write_packet(conn::packet packet);
    void write_raw_data(std::vector<u8>& data);

    i32 read_varint_from_socket();
    void start();
private:
    tcp_connection(boost::asio::io_context& io_context) : socket_(io_context) {}

    void handle_write(const boost::system::error_code& error, size_t bytes_transferred) {}

    tcp::socket socket_;
    packet_dispatcher dispatcher = packet_dispatcher(*this);
};

class tcp_server {
public:
    tcp_server(boost::asio::io_context& io_context, u16 port) : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        start_accept();
    }

private:
    void start_accept();

    void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error);

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};


#endif //SERVER_H
