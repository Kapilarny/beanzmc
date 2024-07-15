cmake_minimum_required(VERSION 3.28)
project(beanzmc)

set(CMAKE_CXX_STANDARD 20)

add_library(beanzmc
        src/defines.h
        src/network/server.cpp
        src/network/server.h
        src/network/packets/data_types.h
        src/network/packets/packet_serializer.h
        src/network/packets/client/client_status_packets.h
        src/network/packets/packet_dispatcher.cpp
        src/network/packets/packet_dispatcher.h
        src/network/packets/server/server_status_packets.h
        src/network/packets/packet_serializer.cpp
        src/network/packets/client/client_login_packets.h
        src/network/packets/server/server_login_packets.h
        src/network/packets/server/server_play_packets.h
        src/util/mem_util.h
        src/network/packets/client/client_play_packets.h
        src/network/packets/handlers/play_packet_handler.h
        src/network/packets/handlers/play_packet_handler.cpp
        src/network/packets/handlers/impl/client_keep_alive.h
        src/nbt/nbt.hpp
)

# -Wshift-count-overflow
target_compile_options(beanzmc PRIVATE -Wno-shift-count-overflow)

target_include_directories(beanzmc PRIVATE ../external/includes src)
target_link_libraries(beanzmc PRIVATE wsock32 ws2_32)