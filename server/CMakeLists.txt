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
)
target_include_directories(beanzmc PRIVATE ../external/includes src)
target_link_libraries(beanzmc PRIVATE wsock32 ws2_32)