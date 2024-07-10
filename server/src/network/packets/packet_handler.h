//
// Created by user on 10.07.2024.
//

#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <stdexcept>

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/phoenix/phoenix.hpp>
using boost::phoenix::arg_names::arg1;

#include "data_types.h"
#include "defines.h"

// TODO: This Fucking Sucks Actually meme
// Just memcpy the entire data into the packet and boom you're done

template<typename T>
inline T read_data(const u8* data, u32& offset) {
    T value = *(T*)(data + offset);
    offset += sizeof(T);
    return value;
}

template<typename T>
inline void write_data(u8* data, u32& offset, T value) {
    *(T*)(data + offset) = value;
    offset += sizeof(T);
}

template<>
inline conn::var_int read_data(const u8* data, u32& offset) {
    conn::var_int value = 0;
    u32 shift = 0;

    while (true) {
        const u8 curr_byte = data[offset++];
        value |= (curr_byte & SEGMENT_BITS) << shift;

        if((curr_byte & CONTINUE_BIT) == 0) break;
        shift += 7;

        if(shift >= 32) throw std::runtime_error("VarInt is too big");
    }

    return value;
}

template<>
inline void write_data(u8* data, u32& offset, conn::var_int value) {
    while(true) {
        if((value & ~SEGMENT_BITS) == 0) {
            data[offset++] = value;
            return;
        }

        data[offset++] = (value & SEGMENT_BITS) | CONTINUE_BIT;
        value = (value >> 7) & ~(1 << 31);
    }
}

template<>
inline conn::var_long read_data(const u8* data, u32& offset) {
    conn::var_long value = 0;
    u32 shift = 0;

    while (true) {
        const u8 curr_byte = data[offset++];
        value |= (i64) (curr_byte & SEGMENT_BITS) << shift;

        if((curr_byte & CONTINUE_BIT) == 0) break;
        shift += 7;

        if(shift >= 64) throw std::runtime_error("VarLong is too big");
    }

    return value;
}

template<>
inline void write_data(u8* data, u32& offset, conn::var_long value) {
    while(true) {
        if((value & ~((i64)SEGMENT_BITS)) == 0) {
            data[offset++] = value;
            return;
        }

        data[offset++] = (value & SEGMENT_BITS) | CONTINUE_BIT;
        value = (value >> 7) & ~(1 << 63);
    }
}

template<typename T>
inline T read_packet(conn::Packet packet) {
    T result{};

    memcpy(result, packet.data, sizeof(T));

    return result;
}


#endif //PACKET_HANDLER_H
