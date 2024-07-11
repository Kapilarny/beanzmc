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

// void copy_memory(u8* to, u8* from, u64 size) {
//     // std::cout << "Copy memory -> " << size << "\n";
//     for(u64 i = 0; i < size; i++) {
//         to[i] = from[i];
//     }
// }



// inline void read_data_typed(u8* data, u32& offset, u8* write_to, u64 type, u64 size) {
//     switch (type) {
//         case reflect::type_id<conn::var_int>(): {
//             conn::var_int value{0};
//             u32 shift = 0;
//
//             while (true) {
//                 const u8 curr_byte = data[offset++];
//                 value.val |= (curr_byte & SEGMENT_BITS) << shift;
//
//                 if((curr_byte & CONTINUE_BIT) == 0) break;
//                 shift += 7;
//
//                 if(shift >= 32) throw std::runtime_error("VarInt is too big");
//             }
//
//             memcpy(write_to, &value, sizeof(conn::var_int));
//         } break;
//         case reflect::type_id<conn::var_long>(): {
//             conn::var_long value = {0};
//             u32 shift = 0;
//
//             while (true) {
//                 const u8 curr_byte = data[offset++];
//                 value.val |= (i64) (curr_byte & SEGMENT_BITS) << shift;
//
//                 if((curr_byte & CONTINUE_BIT) == 0) break;
//                 shift += 7;
//
//                 if(shift >= 64) throw std::runtime_error("VarLong is too big");
//             }
//
//             // std::cout << "Writing VarLong: " << value.val << "\n";
//             memcpy(write_to, (u8*)&value, sizeof(conn::var_long));
//         } break;
//         default:
//             // std::cout << "default value lmao imagine lol\n";
//             memcpy(write_to, data + offset, size);
//             offset += size;
//             break;
//     }
// }

// inline void write_data_typed(u8* data, u32& offset, u64 type, u64 size, u8* write_from) {
//     switch (type) {
//         case reflect::type_id<conn::var_int>(): {
//             conn::var_int value = *(conn::var_int*)write_from;
//             while(true) {
//                 if((value.val & ~SEGMENT_BITS) == 0) {
//                     data[offset++] = value.val;
//                     return;
//                 }
//
//                 data[offset++] = (value.val & SEGMENT_BITS) | CONTINUE_BIT;
//                 value.val = (value.val >> 7) & ~(1 << 31);
//             }
//         } break;
//         case reflect::type_id<conn::var_long>(): {
//             conn::var_long value = *(conn::var_long*)write_from;
//             while(true) {
//                 if((value.val & ~((i64)SEGMENT_BITS)) == 0) {
//                     data[offset++] = value.val;
//                     return;
//                 }
//
//                 data[offset++] = (value.val & SEGMENT_BITS) | CONTINUE_BIT;
//                 value.val = (value.val >> 7) & ~(1 << 63);
//             }
//         } break;
//         default:
//             memcpy(data + offset, write_from, size);
//             offset += size;
//             break;
//     }
// }

std::vector<u8> var_int_generate(i32 val) {
    std::vector<u8> result{};

    while(true) {
        if((val & ~SEGMENT_BITS) == 0) {
            result.push_back(val);
            return result;
        }

        result.push_back((val & SEGMENT_BITS) | CONTINUE_BIT);
        val = (val >> 7) & ~(1 << 31);
    }

    return result;
}

template<typename T>
inline void write_data_typed(u8* data, u32& offset, T value) {
    memcpy(data + offset, &value, sizeof(T));
    offset += sizeof(T);
}

template<>
inline void write_data_typed(u8* data, u32& offset, conn::var_int value) {
    while(true) {
        if((value.val & ~SEGMENT_BITS) == 0) {
            data[offset++] = value.val;
            return;
        }

        data[offset++] = (value.val & SEGMENT_BITS) | CONTINUE_BIT;
        value.val = (value.val >> 7) & ~(1 << 31);
    }
}

template<>
inline void write_data_typed(u8* data, u32& offset, conn::var_long value) {
    while(true) {
        if((value.val & ~((i64)SEGMENT_BITS)) == 0) {
            data[offset++] = value.val;
            return;
        }

        data[offset++] = (value.val & SEGMENT_BITS) | CONTINUE_BIT;
        value.val = (value.val >> 7) & ~(1 << 63);
    }
}

template<>
inline void write_data_typed(u8* data, u32& offset, conn::string value) {
    write_data_typed(data, offset, value.length);
    memcpy(data + offset, value.data, value.length.val);
    offset += value.length.val;
}

template<typename T>
inline void read_data_typed(const u8* data, u32& offset, u8* write_to, T type) {
    memcpy(write_to, data + offset, sizeof(T));
    offset += sizeof(T);
}

template<>
inline void read_data_typed(const u8* data, u32& offset, u8* write_to, conn::var_int type) {
    conn::var_int value{0};
    u32 shift = 0;

    while (true) {
        const u8 curr_byte = data[offset++];
        value.val |= (curr_byte & SEGMENT_BITS) << shift;

        if((curr_byte & CONTINUE_BIT) == 0) break;
        shift += 7;

        if(shift >= 32) throw std::runtime_error("VarInt is too big");
    }

    memcpy(write_to, &value, sizeof(conn::var_int));
}

template<>
inline void read_data_typed(const u8* data, u32& offset, u8* write_to, conn::var_long type) {
    conn::var_long value = {0};
    u32 shift = 0;

    while (true) {
        const u8 curr_byte = data[offset++];
        value.val |= (i64) (curr_byte & SEGMENT_BITS) << shift;

        if((curr_byte & CONTINUE_BIT) == 0) break;
        shift += 7;

        if(shift >= 64) throw std::runtime_error("VarLong is too big");
    }

    memcpy(write_to, (u8*)&value, sizeof(conn::var_long));
}

template<>
inline void read_data_typed(const u8* data, u32& offset, u8* write_to, conn::string type) {
    conn::string value{};

    conn::var_int length{0};
    read_data_typed(data, offset, (u8*)&length, length);

    value.length = length;

    value.data = new char[length.val];
    memcpy(&value, data + offset, sizeof(conn::string));

    memcpy(write_to, &value, sizeof(conn::string));
    offset += length.val;
}

template<typename T>
inline T read_packet_data(conn::packet packet) {
    T result{};

    u32 offset = 0;
    u32 struct_offset = 0;
    reflect::for_each([&](auto I) {
        // For some reason, reflect::offset_of<I>(result) is not working properly so I have to do this hacky thing
        while(struct_offset % reflect::align_of<I>(result) != 0) struct_offset++;
        // std::cout << "Offset: " << struct_offset << "\n";
        read_data_typed(packet.data, offset, (u8*)&result + struct_offset, reflect::get<I>(result));
        struct_offset += reflect::size_of<I>(result);
    }, result);

    return result;
}

template<typename T>
inline conn::packet write_packet_data(T packet, i32 packetID) {
    conn::packet result{};
    result.packetID.val = packetID;

    result.data = new u8[sizeof(T)]; // TODO: Make sure this memory is freed

    u32 offset = 0;
    reflect::for_each([&](auto I) {
        write_data_typed(result.data, offset, reflect::get<I>(packet));
    }, packet);

    result.length.val = (i32) offset;

    return result;
}


#endif //PACKET_HANDLER_H
