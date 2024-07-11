#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "defines.h"
#include "network/packets/data_types.h"
#include "network/packets/packet_handler.h"
#include "network/server.h"

struct TestPacket {
    conn::var_int test_var_int;
    i8 test_byte;
    i32 test_int;
    conn::var_long test_var_long;
};

int main() {
    try {
        boost::asio::io_context io_context;
        tcp_server server(io_context, 8080);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}