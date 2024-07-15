//
// Created by user on 14.07.2024.
//

#ifndef SERVER_PLAY_PACKETS_H
#define SERVER_PLAY_PACKETS_H

#include "network/packets/packet_serializer.h"
#include "network/packets/data_types.h"

struct PlayerInfoPacket_AddPlayer {
    conn::var_int action;
    struct AddPlayerElement {
        conn::uuid uuid{};
        conn::string name;
        struct Property {
            conn::string name;
            conn::string value;
            conn::optional_string signature;
        };
        conn::varint_prefixed_list<Property> properties;
        conn::var_int gamemode;
        conn::var_int ping;
        conn::optional_string display_name;
    };

    conn::varint_prefixed_list<AddPlayerElement> elements;
};

struct ServerJoinGamePacket {
    i32 entity_id{};
    bool is_hardcore{};
    u8 gamemode{}; // 0 - survival, 1 - creative, 2 - adventure, 3 - spectator
    i8 previous_gamemode{}; // -1 if no previous gamemode
    conn::varint_prefixed_list<conn::identifier> world_names;
    // TODO: finish
};

struct ServerPlayerPositionAndLookPacket {
    f64 x{}, y{}, z{};
    f32 yaw{}, pitch{};
    struct Flags {
        bool x: 1;
        bool y: 1;
        bool z: 1;
        bool y_rot: 1;
        bool x_rot: 1;
        // 3 bits unused
    } flags{};
    conn::var_int teleport_id{};
};

struct ServerTimeUpdatePacket {
    i64 world_age{};
    i64 time_of_day{};
};

struct ServerKeepAlivePacket {
    i64 id{};
};

#endif //SERVER_PLAY_PACKETS_H
