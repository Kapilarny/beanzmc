//
// Created by user on 15.07.2024.
//

#include "mc_state_handler.h"

#include "packets/server/server_play_packets.h"

// TODO: make conn::string && any allocated variables for deserialization/serialization **not** leak memory :)

#include "server.h"

mc_state_handler::mc_state_handler(tcp_connection* connection) : connection(connection) {}

void mc_state_handler::handle_connection_start() {
    // Send `Join Game` packet

    // Send `Plugin Message` packet with `minecraft:brand` channel and server brand
    ServerPluginMessagePacket plugin_message_packet = { .channel = string_gen("minecraft:brand"), .data = {.length = 5, .data = (u8*)"beanz"} };

    conn::packet packet = write_packet_data(plugin_message_packet, 0x17);
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
