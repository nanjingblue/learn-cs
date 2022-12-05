link_libraries(ws2_32 wsock32)

add_executable(tcp_cs_client client.cpp)
add_executable(tcp_cs_server server.cpp)
