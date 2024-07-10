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

namespace conn
{
    // Data types
    typedef i32 var_int;
    typedef i64 var_long;

    typedef struct string {
        var_int length;
        char* data;
    } String;

    typedef String chat;
    typedef String identifier;

    typedef struct position {
        big_int32_t x;
        big_int32_t y;
        big_int32_t z;
    } Position;

    typedef big_int8_t angle;

    typedef struct uuid {
        big_int64_t most;
        big_int64_t least;
    } UUID;

    // Packets
    typedef struct packet {
        var_int length;
        var_int packetID;
        u8* data;
    } Packet;

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
    typedef enum HandshakeEnum {
        STATUS = 1,
        LOGIN = 2
    } HandshakeEnum;
} // namespace Conn

#endif //DATA_TYPES_H
