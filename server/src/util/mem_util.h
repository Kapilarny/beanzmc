//
// Created by user on 14.07.2024.
//

#ifndef MEM_UTIL_H
#define MEM_UTIL_H

#include "defines.h"

inline void rev_memcpy(void* to, const void* from, u64 size) {
    for (size_t i = 0; i < size; i++) {
        ((u8*)to)[i] = ((u8*)from)[size - i - 1];
    }
}

#endif //MEM_UTIL_H
