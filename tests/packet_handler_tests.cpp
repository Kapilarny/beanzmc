//
// Created by user on 10.07.2024.
//

#include <gtest/gtest.h>

#include "defines.h"
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

    auto test_packet = read_packet<TestPacket>(p);

    EXPECT_EQ(test_packet.test_var_int.val, 2097151);
    EXPECT_EQ(test_packet.test_byte, 7);
    EXPECT_EQ(test_packet.test_int, 0);
    EXPECT_EQ(test_packet.test_var_long.val, 9223372036854775807);
}