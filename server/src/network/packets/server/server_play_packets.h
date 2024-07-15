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

inline nbt::NBT generate_dimension_codec() {
    // nbt::NBT dimension_type
    // {
    //     "minecraft:dimension_type", nbt::TagCompound {
    //         {"type", "minecraft:dimension_type"},
    //         {"value", std::vector<nbt::TagCompound> {
    //             {
    //                 {"name", "minecraft:overworld"},
    //                 {"id", 0},
    //                 {"element", nbt::TagCompound {
    //                     {"piglin_safe", 0},
    //                     {"natural", 1},
    //                     {"ambient_light", 0.0f},
    //                     {"infiniburn", "minecraft:infiniburn_overworld"},
    //                     {"respawn_anchor_works", 0},
    //                     {"has_skylight", 1},
    //                     {"bed_works", 1},
    //                     {"effects", "minecraft:overworld"},
    //                     {"has_raids", 1},
    //                     {"logical_height", 256},
    //                     {"coordinate_scale", 1.0f},
    //                     {"ultrawarm", 0},
    //                     {"has_ceiling", 0}
    //                 }},
    //             }
    //         }}
    //     }
    // };
    //
    // nbt::NBT biomes = {
    //     "minecraft:worldgen/biome", nbt::TagCompound {
    //         {"type", "minecraft:worldgen/biome"},
    //         {"value", std::vector<nbt::TagCompound> {{
    //             {"name", "minecraft:the_void"},
    //             {"id", 127},
    //             {"element", nbt::TagCompound {
    //                 {"precipitation", "none"},
    //                 {"effects", nbt::TagCompound {
    //                     {"sky_color", 8103167},
    //                     {"water_fog_color", 329011},
    //                     {"fog_color", 12638463},
    //                     {"water_color", 4159204},
    //                     {"mood_sound", nbt::TagCompound {
    //                         {"tick_delay", 6000},
    //                         {"offset", 2.0f},
    //                         {"sound", "minecraft:ambient.cave"},
    //                         {"block_search_extent", 8}
    //                     }},
    //                     }}
    //                 }},
    //                 {"depth", 0.1f},
    //                 {"temperature", 0.5f},
    //                 {"scale", 0.2f},
    //             {"downfall", 0.5f},
    //             {"category", "ocean"},
    //             {"temperature_modifier", "frozen"}
    //         }},
    //         }
    //     }
    // };

    nbt::NBT result = {
        nbt::TagCompound {
                {
                    "minecraft:worldgen/biome", nbt::TagCompound {
                        {"type", "minecraft:worldgen/biome"},
                        {"value", std::vector<nbt::TagCompound> {{
                            {"name", "minecraft:the_void"},
                            {"id", 127},
                            {"element", nbt::TagCompound {
                                {"precipitation", "none"},
                                {"effects", nbt::TagCompound {
                                    {"sky_color", 8103167},
                                    {"water_fog_color", 329011},
                                    {"fog_color", 12638463},
                                    {"water_color", 4159204},
                                    {"mood_sound", nbt::TagCompound {
                                        {"tick_delay", 6000},
                                        {"offset", 2.0f},
                                        {"sound", "minecraft:ambient.cave"},
                                        {"block_search_extent", 8}
                                    }},
                                    }}
                            }},
                            {"depth", 0.1f},
                            {"temperature", 0.5f},
                            {"scale", 0.2f},
                        {"downfall", 0.5f},
                        {"category", "ocean"},
                        {"temperature_modifier", "frozen"}
                        }},
                        }
                    }
                },
                {
                    "minecraft:dimension_type", nbt::TagCompound {
                    {"type", "minecraft:dimension_type"},
                    {"value", std::vector<nbt::TagCompound> {
                        {
                            {"name", "minecraft:overworld"},
                            {"id", 0},
                            {"element", nbt::TagCompound {
                                {"piglin_safe", 0},
                                {"natural", 1},
                                {"ambient_light", 0.0f},
                                {"infiniburn", "minecraft:infiniburn_overworld"},
                                {"respawn_anchor_works", 0},
                                {"has_skylight", 1},
                                {"bed_works", 1},
                                {"effects", "minecraft:overworld"},
                                {"has_raids", 1},
                                {"logical_height", 256},
                                {"coordinate_scale", 1.0f},
                                {"ultrawarm", 0},
                                {"has_ceiling", 0}
                            }},
                        }
                    }}
                    }
                }
        }
    };

    return result;
}

inline nbt::NBT generate_dimension_nbt() {
    nbt::NBT result = {

    };
    return result;
}

struct ServerJoinGamePacket {
    i32 entity_id{};
    bool is_hardcore{};
    u8 gamemode{}; // 0 - survival, 1 - creative, 2 - adventure, 3 - spectator
    i8 previous_gamemode{}; // -1 if no previous gamemode
    conn::varint_prefixed_list<conn::identifier> world_names;
    nbt::NBT dimension_codec = generate_dimension_codec();
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
