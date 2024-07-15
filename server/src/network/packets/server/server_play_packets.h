//
// Created by user on 14.07.2024.
//

#ifndef SERVER_PLAY_PACKETS_H
#define SERVER_PLAY_PACKETS_H

#include "mc/mc_entities.h"
#include "nbt/nbt.hpp"
#include "network/packets/data_types.h"
#include "network/packets/packet_serializer.h"

// TODO: use namespaces bruv

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
    nbt::NBT dimension_codec = {};
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

struct ServerPluginMessagePacket {
    conn::string channel;
    conn::len_derived_byte_array data;
};

struct ServerEntityStatusPacket {
    i32 entity_id{};
    mc::EntityStatus status{};
};

struct ServerDifficultyPacket {
    u8 difficulty{}; // 0 - peaceful, 1 - easy, 2 - normal, 3 - hard
    bool locked{};
};

struct ServerPlayerAbilitiesPacket {
    struct Flags {
        bool invulnerable: 1;
        bool flying: 1;
        bool allow_flying: 1;
        bool creative_mode: 1;
        // 4 bits unused
    } flags{};
    f32 flying_speed = 0.05f;
    f32 field_of_view_modifier = 0.1f;
};

struct ServerHeldItemChangePacket {
    u8 slot{}; // 0 - 8
};

#endif //SERVER_PLAY_PACKETS_H
