cmake_minimum_required(VERSION 3.23)
project(test_net)

set(CMAKE_CXX_STANDARD 98)

add_subdirectory(broadcast)
add_subdirectory(multicast)
add_subdirectory(tcp_cs)
add_subdirectory(udp)

add_executable(test_net main.cpp)
