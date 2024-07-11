//
// Created by user on 10.07.2024.
//

#include <gtest/gtest.h>

#include "defines.h"
#include "network/packets/client/client_status_packets.h"
#include "network/packets/data_types.h"
#include "network/packets/packet_handler.h"

TEST(packet_handler_tests, read_packet_sample) {
    struct TestPacket {
        conn::var_int test_var_int;
        i8 test_byte;
        i32 test_int;
        conn::var_long test_var_long;
    };

    std::vector<u8> test_packet_data = {0xff, 0xff, 0x7f, 0x07, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f};
    conn::packet p{.data = test_packet_data.data()};

    auto test_packet = read_packet_data<TestPacket>(p);

    EXPECT_EQ(test_packet.test_var_int.val, 2097151);
    EXPECT_EQ(test_packet.test_byte, 7);
    EXPECT_EQ(test_packet.test_int, 1);
    EXPECT_EQ(test_packet.test_var_long.val, 9223372036854775807);
}

TEST(packet_handler_tests, write_packet_sample) {
    struct TestPacket {
        conn::var_int test_var_int = {2097151};
        i8 test_byte = 7;
        i32 test_int = 1;
        conn::var_long test_var_long = {9223372036854775807};
    };

    conn::packet p = write_packet_data<TestPacket>(TestPacket{}, 69);

    std::vector<u8> test_packet_data = {0xff, 0xff, 0x7f, 0x07, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f};
    EXPECT_EQ(p.length.val, test_packet_data.size());
    EXPECT_EQ(p.packetID.val, 69);
    EXPECT_EQ(memcmp(p.data, test_packet_data.data(), test_packet_data.size()), 0);
}

TEST(packet_handler_tests, generate_varint) {
    std::vector<u8> one = var_int_generate(9);
    std::vector<u8> two = var_int_generate(2);
    std::vector<u8> sample = var_int_generate(25565);

    // Print hex
    for (auto i : one) {
        std::cout << std::hex << (int)i << " ";
    }
    std::cout << "\n";

    for (auto i : two) {
        std::cout << std::hex << (int)i << " ";
    }
    std::cout << "\n";

    for (auto i : sample) {
        std::cout << std::hex << (int)i << " ";
    }
    std::cout << "\n";
}

TEST(packet_handler_tests, read_handshake_packet) {
    std::vector<u8> test_packet_data = {0x80, 0x01, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x90, 0x1F, 0x01};
    conn::packet p{.data = test_packet_data.data()};

    auto test_packet = read_packet_data<HandshakePacket>(p);

    EXPECT_EQ(test_packet.protocol_version.val, 128);
    EXPECT_EQ(test_packet.server_address.length.val, 9);
    EXPECT_EQ(memcmp(test_packet.server_address.data, (char*)"localhost", 9), 0);
    EXPECT_EQ(test_packet.server_port, 8080);
    EXPECT_EQ(test_packet.next_state.val, 1);
}

TEST(packet_handler_tests, write_handshake_packet) {
    HandshakePacket test_packet = {
        .protocol_version = {128},
        .server_address = {.length = {9}, .data = (char*)"localhost"},
        .server_port = 8080,
        .next_state = {1}
    };

    conn::packet p = write_packet_data<HandshakePacket>(test_packet, conn::HANDSHAKE);

    std::vector<u8> test_packet_data = {0x80, 0x01, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x90, 0x1F, 0x01};

    EXPECT_EQ(p.packetID.val, conn::HANDSHAKE);
    EXPECT_EQ(p.length.val, test_packet_data.size());
    EXPECT_EQ(memcmp(p.data, test_packet_data.data(), test_packet_data.size()), 0);
}
