include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(beanzmc_server_test packet_handler_tests.cpp)
target_link_libraries(beanzmc_server_test PRIVATE GTest::gtest_main beanzmc)
target_include_directories(beanzmc_server_test PRIVATE ../server/src ../external/includes)

include(GoogleTest)
gtest_discover_tests(beanzmc_server_test)

#add_library(GTest::GTest INTERFACE IMPORTED)
#target_link_libraries(GTest::GTest INTERFACE gtest_main)

#add_executable(beanzmc_server_test packet_handler_tests.cpp)
#target_link_libraries(beanzmc_server_test PRIVATE beanzmc GTest::GTest)

#add_test(beanzmc_server_test packet_handler_tests.cpp)