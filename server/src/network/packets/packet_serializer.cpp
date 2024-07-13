//
// Created by user on 11.07.2024.
//

#include "packet_serializer.h"

std::vector<u8> var_int_gen_arr(i32 val) {
    std::vector<u8> result{};

    while (true) {
        if ((val & ~SEGMENT_BITS) == 0) {
            result.push_back(val);
            return result;
        }

        result.push_back((val & SEGMENT_BITS) | CONTINUE_BIT);
        val = ((u32)val) >> 7;
    }
}

conn::string string_gen(std::string str) {
    conn::string result{};
    result.length.val = str.length();
    result.data = new char[str.length()];

    memcpy(result.data, str.c_str(), str.length());

    return result;
}

