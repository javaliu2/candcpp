project(30days_make_cpp_server)
add_executable(1_server
1_basic_socket_communication/server.cpp
)
add_executable(1_client
1_basic_socket_communication/client.cpp
)
add_executable(1_host_net_order
1_basic_socket_communication/host_net_order.cpp
)

add_executable(2_server
2_say_hello/server.cpp 2_say_hello/util.cpp)

add_executable(2_client
2_say_hello/client.cpp 2_say_hello/util.cpp)

add_executable(3_client
3_epoll/client.cpp 3_epoll/util.cpp)

add_executable(3_server_block
3_epoll/server_block.cpp 3_epoll/util.cpp
)

add_executable(3_server_nonblock
3_epoll/server_nonblock.cpp 3_epoll/util.cpp
)