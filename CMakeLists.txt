cmake_minimum_required(VERSION 3.28)
project(beanzmc)

set(CMAKE_CXX_STANDARD 17)

add_executable(beanzmc src/main.cpp
        src/defines.h
        src/network/server.cpp
        src/network/server.h
        src/network/packets/data_types.h
        src/network/packets/packet_serializer.h)
target_include_directories(beanzmc PRIVATE external/includes src/)
target_link_libraries(beanzmc PRIVATE wsock32 ws2_32)