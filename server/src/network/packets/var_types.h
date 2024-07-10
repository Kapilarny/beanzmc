//
// Created by user on 10.07.2024.
//

#ifndef VAR_TYPES_H
#define VAR_TYPES_H

#include <stdexcept>

#include "data_types.h"
#include "defines.h"

class var_int {
public:
    var_int() : value(0) {}
    var_int(i32 value) : value(value) {}

    static var_int from_bytes(const u8* data, u32& offset) {
        var_int value = 0;
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

    void to_bytes(u8* data, u32& offset) const {
        i32 value = this->value;
        while(true) {
            if((value & ~SEGMENT_BITS) == 0) {
                data[offset++] = value;
                return;
            }

            data[offset++] = (value & SEGMENT_BITS) | CONTINUE_BIT;
            value = (value >> 7) & ~(1 << 31);
        }
    }

    // Operators
    var_int operator+(const var_int& other) const {
        return value + other.value;
    }

    var_int operator-(const var_int& other) const {
        return value - other.value;
    }

    var_int operator*(const var_int& other) const {
        return value * other.value;
    }

    var_int operator/(const var_int& other) const {
        return value / other.value;
    }

    var_int& operator+=(const var_int& other) {
        value += other.value;
        return *this;
    }

    var_int& operator-=(const var_int& other) {
        value -= other.value;
        return *this;
    }

    var_int &operator*=(const var_int &other) {
        value *= other.value;
        return *this;
    }

    var_int &operator/=(const var_int &other) {
        value /= other.value;
        return *this;
    }

    friend bool operator==(const var_int &lhs, const var_int &rhs) { return lhs.value == rhs.value; }
    friend bool operator!=(const var_int &lhs, const var_int &rhs) { return !(lhs == rhs); }
private:
    i32 value;
};

#endif //VAR_TYPES_H
