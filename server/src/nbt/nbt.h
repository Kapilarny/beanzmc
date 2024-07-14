//
// Created by user on 14.07.2024.
//

#ifndef NBT_H
#define NBT_H

#include <string>
#include <variant>
#include <vector>
#include <concepts>

#include <boost/container/map.hpp>

#include "defines.h"
#include "util/mem_util.h"

namespace nbt {
    enum class TagType : u8 {
        TAG_End = 0,
        TAG_Byte = 1,
        TAG_Short = 2,
        TAG_Int = 3,
        TAG_Long = 4,
        TAG_Float = 5,
        TAG_Double = 6,
        TAG_Byte_Array = 7,
        TAG_String = 8,
        TAG_List = 9,
        TAG_Compound = 10,
        TAG_Int_Array = 11,
        TAG_Long_Array = 12
    };

    struct nbt_data {
        std::string name;
        union {
            boost::container::map<std::string, nbt_data> children;

        };
    };

    struct TagList;
    struct Tag;

    typedef std::nullptr_t tag_end;

    typedef i8 tag_byte;
    typedef i16 tag_short;
    typedef i32 tag_int;
    typedef i64 tag_long;
    typedef f32 tag_float;
    typedef f64 tag_double;

    typedef std::vector<tag_byte> tag_byte_array;
    typedef std::vector<tag_int> tag_int_array;
    typedef std::vector<tag_long> tag_long_array;

    typedef std::string tag_string;

    typedef boost::container::map<std::string, Tag> tag_compound;

    struct vector_reader {
        u64 index = 0;
        std::vector<u8> &data;
    };

    template <std::integral T>
    T decode(vector_reader& reader) {
        T val;

        // Read the value from the data with the correct endianness
        rev_memcpy(&val, reader.data.data() + reader.index, sizeof(T));
        reader.index += sizeof(T);

        return val;
    }


    class nbt {
    public:
        explicit nbt(const std::vector<u8> &from_bytes);
        explicit nbt(nbt_data from_data);

        std::vector<u8> serialize();

        [[nodiscard]] nbt_data &get_data() { return data; }

    private:
        void parse(const std::vector<u8> &from_bytes);

        nbt_data data;
    };
}


#endif //NBT_H
