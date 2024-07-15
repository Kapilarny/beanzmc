//
// Created by user on 10.07.2024.
//

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

// #include <boost/endian/arithmetic.hpp>

#include "defines.h"

#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80

// using namespace boost::endian;

#include <iostream>

namespace conn
{
    // Data types
    struct var_int { i32 val{}; };
    struct var_long { i64 val{}; };

    template<typename T>
    struct varint_prefixed_list {
        var_int length;
        T* data{};
    };

    typedef struct string {
        var_int length;
        char* data{};
    } string;

    struct optional_string {
        bool present = false;
        string value;
    };

    inline void print_string(const string& str) {
        for(int i = 0; i < str.length.val; i++) {
            std::cout << str.data[i];
        }

        std::cout << std::endl;
    }

    typedef string chat;
    typedef string identifier;

    typedef struct position {
        i32 x;
        i32 y;
        i32 z;
    } position;

    struct var_int_prefixed_byte_array {
        var_int length;
        u8* data{};
    };

    // TODO: WARNING: Reading this type is not implemented yet! only write works
    struct len_derived_byte_array {
        u32 length{};
        u8* data{};
    };

    typedef i8 angle;

    typedef struct uuid {
        i64 most;
        i64 least;
    } UUID;

    // Packets
    typedef struct packet {
        var_int length;
        var_int packetID;
        u8* data{};
    } packet;

    // Enums
    enum class ConnectionState {
        HANDSHAKE = 0,
        STATUS = 1,
        LOGIN = 2,
        PLAY = 3
    };
} // namespace Conn

#endif //DATA_TYPES_H
