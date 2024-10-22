//
// Created by user on 10.07.2024.
//

#ifndef PACKET_SERIALIZER_H
#define PACKET_SERIALIZER_H

#include <stdexcept>
#include <vector>
#include <sstream>

#include <reflect>

#include "data_types.h"
#include "defines.h"
#include "nbt/nbt.hpp"
#include "util/mem_util.h"

// TODO: Cleanup this file and make it more readable

// todo: fullon rewrite this

conn::string string_gen(std::string str);
std::vector<u8> var_int_gen_arr(i32 val);

template<typename T>
inline void write_data_typed(u8* data, u32& offset, const T& value) {
    std::cout << "write_data_typed<T> triggered\n";
    // memcpy(data + offset, &value, sizeof(T));

    // Swap endianness
    rev_memcpy(data + offset, (u8*)&value, sizeof(T));

    offset += sizeof(T);
}

template<>
inline void write_data_typed(u8* data, u32& offset, const conn::uuid& value) {
    std::cout << "write_data_typed<conn::uuid> triggered\n";
    write_data_typed(data, offset, value.most);
    write_data_typed(data, offset, value.least);
}

template<>
inline void write_data_typed(u8* data, u32& offset, const conn::var_int& value) {
    std::cout << "write_data_typed<conn::var_int> triggered\n";
    i32 val = value.val;
    while(true) {
        if((value.val & ~SEGMENT_BITS) == 0) {
            data[offset++] = val;
            return;
        }

        data[offset++] = (val & SEGMENT_BITS) | CONTINUE_BIT;
        val = ((u32)val) >> 7;
    }
}

template<>
inline void write_data_typed(u8* data, u32& offset, const conn::var_long& value) {
    std::cout << "write_data_typed<conn::var_long> triggered\n";
    i64 val = value.val;
    while(true) {
        if((value.val & ~((i64)SEGMENT_BITS)) == 0) {
            data[offset++] = val;
            return;
        }

        data[offset++] = (val & SEGMENT_BITS) | CONTINUE_BIT;
        val = ((u64)val) >> 7;
    }
}

template<>
inline void write_data_typed(u8* data, u32& offset, const conn::string& value) {
    std::cout << "write_data_typed<conn::string> triggered\n";
    write_data_typed(data, offset, value.length);
    memcpy(data + offset, value.data, value.length.val);
    offset += value.length.val;
}

template<>
inline void write_data_typed(u8* data, u32& offset, const conn::optional_string& value) {
    std::cout << "write_data_typed<conn::optional_string> triggered\n";
    write_data_typed(data, offset, value.present);
    if (value.present) {
        write_data_typed(data, offset, value.value);
    }
}

template<typename T>
bool is_non_extendable_parser_type() {
    return typeid(T) == typeid(conn::var_int) || typeid(T) == typeid(conn::var_long) || typeid(T) == typeid(conn::string) || typeid(T) == typeid(conn::optional_string) || typeid(T) == typeid(conn::uuid) || typeid(T) == typeid(conn::var_int_prefixed_byte_array);
}

template<typename T>
inline void write_data_typed(u8* data, u32& offset, const conn::varint_prefixed_list<T>& value) {
    std::cout << "write_data_typed<conn::varint_prefixed_list<T>> triggered\n";
    write_data_typed(data, offset, value.length);

    for (i32 i = 0; i < value.length.val; i++) {
        auto element = value.data[i];
        if(is_non_extendable_parser_type<T>()) {
            reflect::for_each([&](auto I) {
                write_data_typed(data, offset, reflect::get<I>(element));
            }, element);
        } else {
            write_data_typed(data, offset, element);
        }
    }
}

template<>
inline void write_data_typed(u8* data, u32& offset, const conn::var_int_prefixed_byte_array& value) {
    std::cout << "write_data_typed<conn::var_int_prefixed_byte_array> triggered\n";
    write_data_typed(data, offset, value.length);
    memcpy(data + offset, value.data, value.length.val);
    offset += value.length.val;
}

template<>
inline void write_data_typed(u8* data, u32& offset, const conn::len_derived_byte_array& value) {
    std::cout << "write_data_typed<conn::len_derived_byte_array> triggered\n";
    memcpy(data + offset, value.data, value.length);
    offset += value.length;
}

template<>
inline void write_data_typed(u8* data, u32& offset, const nbt::NBT& value) {
    std::cout << "write_data_typed<nbt::NBT> triggered\n";
    std::ostringstream stream;
    value.encode(stream);

    // copy stream to data
    auto str = stream.str();

    memcpy(data + offset, str.c_str(), str.length());
    offset += str.length();
}

template<typename T>
inline void read_data_typed(const u8* data, u32& offset, u8* write_to, T type) {
    // memcpy(write_to, data + offset, sizeof(T));

    // Swap endianness
    rev_memcpy(write_to, data + offset, sizeof(T));

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

template<>
inline void read_data_typed(const u8* data, u32& offset, u8* write_to, conn::optional_string type) {
    conn::optional_string value{};

    bool present;
    read_data_typed(data, offset, (u8*)&present, present);

    value.present = present;

    if (value.present) {
        conn::string str{};
        read_data_typed(data, offset, (u8*)&str, str);
        value.value = str;
    }

    memcpy(write_to, &value, sizeof(conn::optional_string));
}

template<>
inline void read_data_typed(const u8* data, u32& offset, u8* write_to, conn::uuid type) {
    conn::uuid value{};
    read_data_typed(data, offset, (u8*)&value.most, value.most);
    read_data_typed(data, offset, (u8*)&value.least, value.least);

    memcpy(write_to, &value, sizeof(conn::uuid));
}

template<>
inline void read_data_typed(const u8* data, u32& offset, u8* write_to, conn::var_int_prefixed_byte_array type) {
    conn::var_int length{0};
    read_data_typed(data, offset, (u8*)&length, length);

    type.length = length;
    type.data = new u8[length.val];
    memcpy(type.data, data + offset, length.val);

    memcpy(write_to, &type, sizeof(conn::var_int_prefixed_byte_array));
    offset += length.val;
}

template<typename T>
inline void read_data_typed(const u8* data, u32& offset, u8* write_to, conn::varint_prefixed_list<T> type) {
    conn::var_int length{0};
    read_data_typed(data, offset, (u8*)&length, length);

    type.length = length;
    type.data = new T[length.val];

    for (i32 i = 0; i < length.val; i++) {
        read_data_typed(data, offset, (u8*)&type.data[i], type.data[i]);
    }

    memcpy(write_to, &type, sizeof(conn::varint_prefixed_list<T>));
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
constexpr inline u64 get_field_size(T& field) {
    return sizeof(T);
}

// This is unefficient AF, TODO: Rewrite
template<>
inline u64 get_field_size(nbt::NBT& field) {
    std::ostringstream stream;
    field.encode(stream);

    std::cout << "NBT size: " << stream.str().length() << "\n";
    // print nbt bytes
    for (u32 i = 0; i < stream.str().length(); i++) {
        printf("%02X ", (u8)stream.str()[i]);
    }

    return stream.str().length();
}

template<typename T>
constexpr inline u64 get_field_size(conn::varint_prefixed_list<T>& field) {
    u64 size = 0;
    for (i32 i = 0; i < field.length.val; i++) {
        auto& element = field.data[i];
        reflect::for_each([&](auto I) {
            size += get_field_size(reflect::get<I>(element));
        }, element);
    }

    return size + get_field_size(field.length);
}

template<>
inline u64 get_field_size(conn::string& field) {
    return field.length.val + sizeof(conn::var_int);
}

template<>
inline u64 get_field_size(conn::optional_string& field) {
    return field.present ? get_field_size(field.value) + sizeof(bool) : sizeof(bool);
}

template<typename T>
inline u64 get_packet_size(T& packet) {
    u64 size = 0;
    reflect::for_each([&](auto I) {
        // std::cout << "Size of field: " << get_field_size(reflect::get<I>(packet)) << "\n";
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
