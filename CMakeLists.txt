cmake_minimum_required(VERSION 3.28)
project(beanzmc_server)

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory(server)
add_subdirectory(tests)

add_executable(beanzmc_server src/main.cpp)
target_include_directories(beanzmc_server PRIVATE external/includes src server/src)
target_link_libraries(beanzmc_server PRIVATE beanzmc bcrypt)