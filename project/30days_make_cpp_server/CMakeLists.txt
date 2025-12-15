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

add_executable(3_server
3_epoll/server.cpp 3_epoll/util.cpp
)

add_executable(4_server
4_hello_class/server.cpp 4_hello_class/Epoll.cpp 4_hello_class/BaseSocket.cpp 4_hello_class/ServerSocket.cpp 4_hello_class/ClientSocket.cpp 4_hello_class/InetAddress.cpp 4_hello_class/util.cpp
4_hello_class/Channel.cpp 4_hello_class/EventLoop.cpp)  
# 因为修改了Epoll, 新增了Channel, 所以要把Channel.cpp加上, Channel又涉及到EventLoop, 所以EventLoop.cpp也要加上, 否则会编译报错，显示找不到函数的定义

add_executable(4_client
4_hello_class/client.cpp 4_hello_class/BaseSocket.cpp 4_hello_class/ClientSocket.cpp 4_hello_class/InetAddress.cpp 4_hello_class/util.cpp
)

add_executable(4_client2
4_hello_class/client2.cpp 4_hello_class/BaseSocket.cpp 4_hello_class/ClientSocket.cpp 4_hello_class/InetAddress.cpp 4_hello_class/util.cpp
4_hello_class/ThreadPool.cpp)

add_executable(4_server2
4_hello_class/EventLoop.cpp 4_hello_class/Channel.cpp 4_hello_class/Server.cpp
4_hello_class/server2.cpp 4_hello_class/Epoll.cpp 4_hello_class/BaseSocket.cpp 
4_hello_class/ServerSocket.cpp 4_hello_class/ClientSocket.cpp 4_hello_class/InetAddress.cpp 
4_hello_class/util.cpp 4_hello_class/Acceptor.cpp 4_hello_class/Connection.cpp 4_hello_class/ThreadPool.cpp)

add_executable(5_client
5_main_sub_reactor/client.cpp 5_main_sub_reactor/BaseSocket.cpp 5_main_sub_reactor/ClientSocket.cpp 5_main_sub_reactor/InetAddress.cpp 5_main_sub_reactor/util.cpp
5_main_sub_reactor/ThreadPool.cpp)

add_executable(5_server
5_main_sub_reactor/EventLoop.cpp 5_main_sub_reactor/Channel.cpp 5_main_sub_reactor/Server.cpp
5_main_sub_reactor/server.cpp 5_main_sub_reactor/Epoll.cpp 5_main_sub_reactor/BaseSocket.cpp 
5_main_sub_reactor/ServerSocket.cpp 5_main_sub_reactor/ClientSocket.cpp 5_main_sub_reactor/InetAddress.cpp 
5_main_sub_reactor/util.cpp 5_main_sub_reactor/Acceptor.cpp 5_main_sub_reactor/Connection.cpp 5_main_sub_reactor/ThreadPool.cpp)