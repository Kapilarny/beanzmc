//
// Created by user on 10.07.2024.
//

#ifndef PACKET_SERIALIZER_H
#define PACKET_SERIALIZER_H

#include <stdexcept>
#include <vector>

#include <reflect>

#include "data_types.h"
#include "defines.h"

conn::string string_gen(std::string str);
std::vector<u8> var_int_gen_arr(i32 val);

template<typename T>
inline void write_data_typed(u8* data, u32& offset, T value) {
    // memcpy(data + offset, &value, sizeof(T));

    // Swap endianness
    for (u32 i = 0; i < sizeof(T); i++) {
        data[offset + sizeof(T) - i - 1] = *((u8*)&value + i);
    }

    offset += sizeof(T);
}

template<>
inline void write_data_typed(u8* data, u32& offset, conn::uuid value) {
    write_data_typed(data, offset, value.most);
    write_data_typed(data, offset, value.least);
}

template<>
inline void write_data_typed(u8* data, u32& offset, conn::var_int value) {
    while(true) {
        if((value.val & ~SEGMENT_BITS) == 0) {
            data[offset++] = value.val;
            return;
        }

        data[offset++] = (value.val & SEGMENT_BITS) | CONTINUE_BIT;
        value.val = ((u32)value.val) >> 7;
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
        value.val = ((u64)value.val) >> 7;
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
    // memcpy(write_to, data + offset, sizeof(T));

    // Swap endianness
    for (u32 i = 0; i < sizeof(T); i++) {
        write_to[i] = data[offset + sizeof(T) - i - 1];
    }

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
    memcpy(value.data, data + offset, value.length.val);

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
u64 get_field_size(T& field) {
    return sizeof(T);
}

template<>
inline u64 get_field_size(conn::string& field) {
    return field.length.val + sizeof(conn::var_int);
}

template<typename T>
inline u64 get_packet_size(T& packet) {
    u64 size = 0;
    reflect::for_each([&](auto I) {
        std::cout << "Size of field: " << get_field_size(reflect::get<I>(packet)) << "\n";
        size += get_field_size(reflect::get<I>(packet));
    }, packet);

    return size;
}

inline u8 get_varint_size(i32 val) {
    u8 size = 0;

    while (true) {
        if ((val & ~SEGMENT_BITS) == 0) {
            size++;
            return size;
        }

        size++;
        val = ((u32)val) >> 7;
    }
}

template<typename T>
inline conn::packet write_packet_data(T packet, i32 packetID) {
    conn::packet result{};
    result.packetID.val = packetID;

    // i am literally such a clueless dumbass
    auto packet_size = get_packet_size(packet);
    result.data = new u8[packet_size]; // TODO: Make sure this memory is freed

    u32 offset = 0;
    reflect::for_each([&](auto I) {
        write_data_typed(result.data, offset, reflect::get<I>(packet));
    }, packet);

    result.length.val = static_cast<i32>(offset + get_varint_size(result.packetID.val));

    std::cout << "Created packet with size: " << std::dec <<  result.length.val << "\n";

    return result;
}


#endif //PACKET_SERIALIZER_H
