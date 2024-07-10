//
// Created by user on 10.07.2024.
//

#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <stdexcept>

#include <boost/fusion/adapted/struct.hpp>
#include <boost/fusion/include/for_each.hpp>
#include <boost/phoenix/phoenix.hpp>

#include "boost/reflect.hpp"
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

// template<>
// inline conn::var_int read_data(const u8* data, u32& offset) {
//     conn::var_int value = 0;
//     u32 shift = 0;
//
//     while (true) {
//         const u8 curr_byte = data[offset++];
//         value |= (curr_byte & SEGMENT_BITS) << shift;
//
//         if((curr_byte & CONTINUE_BIT) == 0) break;
//         shift += 7;
//
//         if(shift >= 32) throw std::runtime_error("VarInt is too big");
//     }
//
//     return value;
// }
//
// template<>
// inline void write_data(u8* data, u32& offset, conn::var_int value) {
//     while(true) {
//         if((value & ~SEGMENT_BITS) == 0) {
//             data[offset++] = value;
//             return;
//         }
//
//         data[offset++] = (value & SEGMENT_BITS) | CONTINUE_BIT;
//         value = (value >> 7) & ~(1 << 31);
//     }
// }
//
// template<>
// inline conn::var_long read_data(const u8* data, u32& offset) {
//     conn::var_long value = 0;
//     u32 shift = 0;
//
//     while (true) {
//         const u8 curr_byte = data[offset++];
//         value |= (i64) (curr_byte & SEGMENT_BITS) << shift;
//
//         if((curr_byte & CONTINUE_BIT) == 0) break;
//         shift += 7;
//
//         if(shift >= 64) throw std::runtime_error("VarLong is too big");
//     }
//
//     return value;
// }
//
// template<>
// inline void write_data(u8* data, u32& offset, conn::var_long value) {
//     while(true) {
//         if((value & ~((i64)SEGMENT_BITS)) == 0) {
//             data[offset++] = value;
//             return;
//         }
//
//         data[offset++] = (value & SEGMENT_BITS) | CONTINUE_BIT;
//         value = (value >> 7) & ~(1 << 63);
//     }
// }

void copy_memory(u8* to, u8* from, u64 size) {
    // std::cout << "Copy memory -> " << size << "\n";
    for(u64 i = 0; i < size; i++) {
        to[i] = from[i];
    }
}

inline void read_data_typed(u8* data, u32& offset, u8* write_to, u64 type, u64 size) {
    std::cout << "write_to ->" << (u64)write_to << "\n";
    switch (type) {
        case reflect::type_id<conn::var_int>(): {
            conn::var_int value{0};
            u32 shift = 0;

            while (true) {
                const u8 curr_byte = data[offset++];
                value.val |= (curr_byte & SEGMENT_BITS) << shift;

                if((curr_byte & CONTINUE_BIT) == 0) break;
                shift += 7;

                if(shift >= 32) throw std::runtime_error("VarInt is too big");
            }

            copy_memory(write_to, (u8*)&value, sizeof(conn::var_int));
        } break;
        case reflect::type_id<conn::var_long>(): {
            conn::var_long value = {0};
            u32 shift = 0;

            while (true) {
                const u8 curr_byte = data[offset++];
                value.val |= (i64) (curr_byte & SEGMENT_BITS) << shift;

                if((curr_byte & CONTINUE_BIT) == 0) break;
                shift += 7;

                if(shift >= 64) throw std::runtime_error("VarLong is too big");
            }

            std::cout << "Writing VarLong: " << value.val << "\n";
            copy_memory(write_to, (u8*)&value, sizeof(conn::var_long));
        } break;
        default:
            // std::cout << "default value lmao imagine lol\n";
            copy_memory(write_to, data + offset, size);
            offset += size;
            break;
    }
}

template<typename T>
inline T read_packet(conn::Packet packet) {
    T result{};

    u32 offset = 0;
    reflect::for_each([&](auto I) {
        // std::cout << reflect::offset_of<I>(result);
        read_data_typed(packet.data, offset, ((u8*)&result) + reflect::offset_of<I>(result), reflect::type_id(reflect::get<I>(result)), reflect::size_of<I>(result));
    }, result);

    return result;
}


#endif //PACKET_HANDLER_H
