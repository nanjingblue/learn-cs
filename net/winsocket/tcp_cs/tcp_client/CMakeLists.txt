cmake_minimum_required(VERSION 3.0)
project(tcp_cs)

set(CMAKE_CXX_STANDARD 14)

link_libraries(ws2_32 wsock32)

include_directories(../include)

add_executable(tcp_client tcp_client.cpp ../include/parser.h ../include/file_helper.h)
