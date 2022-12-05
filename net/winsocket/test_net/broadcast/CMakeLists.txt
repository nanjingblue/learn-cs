link_libraries(ws2_32 wsock32)

add_executable(broadcast_client client.cpp)
add_executable(broadcast_server server.cpp)