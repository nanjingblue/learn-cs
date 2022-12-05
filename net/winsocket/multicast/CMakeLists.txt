cmake_minimum_required(VERSION 3.23)
project(multicast)

set(CMAKE_CXX_STANDARD 14)

link_libraries(ws2_32 wsock32)

add_executable(multicast main.cpp)
add_executable(receiver receiver.cpp)
add_executable(tcli tcli.cpp)
