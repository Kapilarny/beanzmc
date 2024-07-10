//
// Created by user on 10.07.2024.
//

#include <gtest/gtest.h>

#include "defines.h"
#include "network/packets/data_types.h"
#include "network/packets/packet_handler.h"

TEST(packet_handler_tests, VarInt_ReadWrite) {
    std::vector<u8> sample_var_int = {0xff, 0xff, 0xff, 0xff, 0x07};
    u32 offset = 0;
    conn::var_int var_int = read_data<conn::var_int>(sample_var_int.data(), offset);
    EXPECT_EQ(var_int, 2147483647);

    std::vector<u8> write_sample_var_int(5);
    offset = 0;
    write_data<conn::var_int>(write_sample_var_int.data(), offset, var_int);

    EXPECT_EQ(write_sample_var_int, sample_var_int);
}

TEST(packet_handler_tests, VarLong_ReadWrite) {
    std::vector<u8> sample_var_long = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f };
    u32 offset = 0;
    conn::var_long var_long = read_data<conn::var_long>(sample_var_long.data(), offset);
    EXPECT_EQ(var_long, 9223372036854775807);

    std::vector<u8> write_sample_var_long(9);
    offset = 0;
    write_data<conn::var_long>(write_sample_var_long.data(), offset, var_long);

    EXPECT_EQ(write_sample_var_long, sample_var_long);
}