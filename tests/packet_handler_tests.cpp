//
// Created by user on 10.07.2024.
//

#include <gtest/gtest.h>

#include "defines.h"
#include "network/packets/client/client_status_packets.h"
#include "network/packets/data_types.h"
#include "network/packets/packet_serializer.h"
#include "network/packets/server/server_play_packets.h"

TEST(packet_handler_tests, read_packet_sample) {
    struct TestPacket {
        conn::var_int test_var_int;
        i8 test_byte;
        i32 test_int;
        conn::var_long test_var_long;
    };

    std::vector<u8> test_packet_data = {0xff, 0xff, 0x7f, 0x07, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f};
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

    // Print all bytes comparing to the test_packet_data
    std::vector<u8> test_packet_data = {0xff, 0xff, 0x7f, 0x07, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f};

    EXPECT_EQ(p.length.val, test_packet_data.size()+1); // +1 for the packet ID
    EXPECT_EQ(p.packetID.val, 69);
    EXPECT_EQ(memcmp(p.data, test_packet_data.data(), test_packet_data.size()), 0);
}

TEST(packet_handler_tests, read_handshake_packet) {
    std::vector<u8> test_packet_data = {0x80, 0x01, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x1F, 0x90, 0x01};
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

    conn::packet p = write_packet_data<HandshakePacket>(test_packet, 0x00);

    std::vector<u8> test_packet_data = {0x80, 0x01, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x1F, 0x90, 0x01};

    EXPECT_EQ(p.packetID.val, 0x00);
    EXPECT_EQ(p.length.val, test_packet_data.size()+1); // +1 for the packet ID
    EXPECT_EQ(memcmp(p.data, test_packet_data.data(), test_packet_data.size()), 0);
}

TEST(packet_handler_tests, get_size_of_varint_prefixed_list) {
    struct TestElement {
        u32 test_int;
        i8 test_byte;
    }; // 5 bytes

    conn::varint_prefixed_list<TestElement> test_list = {
        .length = {2},
        .data = new TestElement[2]{{.test_int = 1, .test_byte = 2}, {.test_int = 3, .test_byte = 4}}
    };

    u64 size = get_field_size(test_list);
    EXPECT_EQ(size, 2 * 5 + get_field_size(test_list.length));
}

TEST(packet_handler_tests, write_player_info_add_player_packet) {
    PlayerInfoPacket_AddPlayer test_packet = {
        .action = 0x00,
        .elements = {
            .length = 1,
            .data = {
                new PlayerInfoPacket_AddPlayer::AddPlayerElement{
                    .uuid = {},
                    .name = {.length = 5, .data = (char*)"beanz"},
                    .properties = {
                        .length = 0,
                        .data = nullptr
                    },
                    .gamemode = 1,
                    .ping = 30,
                    .display_name = { .present = true, .value = {.length = 5, .data = (char*)"beanz"} }
                }
            }
        }
    };

    conn::packet p = write_packet_data<PlayerInfoPacket_AddPlayer>(test_packet, 0x00); // make it 0x00 to not add additional bytes to length

    std::vector<u8> test_packet_data = {
        // Action
        0x00,
        // Amount of elements
        0x01,
        // UUID
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // Name Length
        0x05,
        // Name contents ("beanz")
        0x62, 0x65, 0x61, 0x6e, 0x7a,
        // Properties Length (not followed with data cause its 0)
        0x00,
        // Gamemode
        0x01,
        // Ping
        0x1e,
        // Display Name present
        0x01,
        // Display Name Length
        0x05,
        // Display Name contents ("beanz")
        0x62, 0x65, 0x61, 0x6e, 0x7a
    };

    EXPECT_EQ(p.length.val - 1, test_packet_data.size()); // -1 for the packet ID
    EXPECT_EQ(memcmp(p.data, test_packet_data.data(), test_packet_data.size()), 0);
}