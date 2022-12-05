link_libraries(ws2_32 wsock32)

add_executable(multicast_client client.cpp)
add_executable(multicast_server server.cpp)