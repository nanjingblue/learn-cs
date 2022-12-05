link_libraries(ws2_32 wsock32)

add_executable(udp_client client.cpp)
add_executable(udp_server server.cpp)
