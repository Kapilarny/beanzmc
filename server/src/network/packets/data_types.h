//
// Created by user on 10.07.2024.
//

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <boost/endian/arithmetic.hpp>

#include "defines.h"

#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80

using namespace boost::endian;

#include <iostream>

namespace conn
{
    // Data types
    struct var_int { i32 val{}; };
    struct var_long { i64 val{}; };

    typedef struct string {
        var_int length;
        char* data;
    } string;

    typedef string chat;
    typedef string identifier;

    typedef struct position {
        big_int32_t x;
        big_int32_t y;
        big_int32_t z;
    } position;

    typedef big_int8_t angle;

    typedef struct uuid {
        big_int64_t most;
        big_int64_t least;
    } UUID;

    // Packets
    typedef struct packet {
        var_int length;
        var_int packetID;
        u8* data{};
    } packet;

    typedef enum StatusPacketType {
        HANDSHAKE = 0x00
    } StatusPacketType;

    typedef enum LoginPacketType {
        LOGIN_START = 0x00,
        LOGIN_SUCCESS = 0x02
    } LoginPacketType;

    typedef enum PlayPacketType {
        SPAWN_ENTITY = 0x00
    } PlayPacketType;

    // Enums
    enum class ConnectionState {
        HANDSHAKE = 0,
        STATUS = 1,
        LOGIN = 2,
        PLAY = 3
    };
} // namespace Conn

#endif //DATA_TYPES_H
