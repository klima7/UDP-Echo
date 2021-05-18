cmake_minimum_required(VERSION 3.19)
project(udp_echo)

set(CMAKE_CXX_STANDARD 14)

add_executable(server server.cpp)
add_executable(client client.cpp)

target_link_libraries(server wsock32 ws2_32)
target_link_libraries(client wsock32 ws2_32)