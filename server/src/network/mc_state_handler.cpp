//
// Created by user on 15.07.2024.
//

#include "mc_state_handler.h"

#include "packets/server/server_play_packets.h"

// TODO: make conn::string && any allocated variables for deserialization/serialization **not** leak memory :)

#include "server.h"
#include "util/data_helper.h"

mc_state_handler::mc_state_handler(tcp_connection* connection) : connection(connection) {}

void mc_state_handler::handle_connection_start() {
    // Send `Join Game` packet
    ServerJoinGamePacket join_game_packet{};
    join_game_packet.entity_id = 8;
    join_game_packet.hashed_seed = 21309920;
    join_game_packet.view_distance.val = 2;
    std::string mc_overworld = "minecraft:overworld";
    join_game_packet.world_name = string_gen(mc_overworld);

    auto* world_names = new conn::identifier[1];
    world_names[0] = string_gen(mc_overworld);

    join_game_packet.world_names = { {1}, world_names };

    conn::packet packet = write_packet_data(join_game_packet, 0x24);

    std::cout << "Writing ServerJoinGamePacket" << std::endl;
    connection->write_packet(packet);

    // Send `Plugin Message` packet with `minecraft:brand` channel and server brand
    ServerPluginMessagePacket plugin_message_packet = { .channel = string_gen("minecraft:brand"), .data = {.length = 5, .data = (u8*)"beanz"} };

    packet = write_packet_data(plugin_message_packet, 0x17);
    connection->write_packet(packet);

    delete[] packet.data;

    // Send `Server Difficulty` packet
    ServerDifficultyPacket difficulty_packet = { .difficulty = 0 };

    packet = write_packet_data(difficulty_packet, 0x0D);
    connection->write_packet(packet);

    delete[] packet.data;

    // Send `Player Abilities` packet
    ServerPlayerAbilitiesPacket player_abilities_packet = { .flags = {}, .flying_speed = 0.05f, .field_of_view_modifier = 0.1f };

    packet = write_packet_data(player_abilities_packet, 0x30);
    connection->write_packet(packet);

    delete[] packet.data;

    // Send `Held Item Change` packet
    ServerHeldItemChangePacket held_item_change_packet = { .slot = 0 };

    packet = write_packet_data(held_item_change_packet, 0x3F);
    connection->write_packet(packet);

    delete[] packet.data;

    // Send `Player Position And Look` packet
    ServerPlayerPositionAndLookPacket player_position_and_look_packet = { .x = 69, .y = 100, .z = 69, .yaw = 0, .pitch = 0, .flags = {}, .teleport_id = 0 };

    packet = write_packet_data(player_position_and_look_packet, 0x34);
    connection->write_packet(packet);

    delete[] packet.data;
}
