//
// Created by user on 15.07.2024.
//

#ifndef MC_ENTITIES_H
#define MC_ENTITIES_H
#include "defines.h"

namespace mc {

    // TODO: Add all status codes
    enum class EntityStatus : u8 {
        SET_OP_PERMISSION_LEVEL_0 = 24,
        SET_OP_PERMISSION_LEVEL_1 = 25,
        SET_OP_PERMISSION_LEVEL_2 = 26,
        SET_OP_PERMISSION_LEVEL_3 = 27,
        SET_OP_PERMISSION_LEVEL_4 = 28,
    };

}

#endif //MC_ENTITIES_H
