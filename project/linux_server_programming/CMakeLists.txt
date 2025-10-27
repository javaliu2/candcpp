project(study_server_programming)
cmake_minimum_required(VERSION 3.22)

add_executable(oob_send
chapter5_basic_api/msg_oob_test_send.cpp
)

add_executable(oob_recv
chapter5_basic_api/msg_oob_test_recv.cpp
)

add_executable(recvbuf_server
chapter5_basic_api/socket_recvbuf_test_server.cpp
)

add_executable(sendbuf_client
chapter5_basic_api/socket_sendbuf_test_client.cpp
)

add_executable(backlog_test
chapter5_basic_api/backlog_test.cpp
)

add_executable(accept_test
chapter5_basic_api/accept_test.cpp
)

add_executable(access_daytime_service
chapter5_basic_api/access_daytime_service.cpp
)

add_executable(cgi_test
chapter6_io_function/cgi_server_test.cpp
)

add_executable(dup_test
chapter6_io_function/dup_test.cpp
)

add_executable(gather_write_test
chapter6_io_function/gather_write.cpp
)

add_executable(sendfile_test
chapter6_io_function/sendfile_test.cpp
)

add_executable(splice_test
chapter6_io_function/splice_test.cpp
)

add_executable(tee_test
chapter6_io_function/tee_test.cpp
)

add_executable(uid_test
chapter7_server_specification/uid_test.cpp
)

add_executable(select_test
chapter9_io_multiplexing/select_test.cpp
)

add_executable(send_oob_data
chapter9_io_multiplexing/send_oob_data.cpp
)

add_executable(LT_ET_test
chapter9_io_multiplexing/LT_ET_test.cpp
)

add_executable(epolloneshot_test
chapter9_io_multiplexing/epolloneshot_test.cpp
)

add_executable(pthread_test
chapter9_io_multiplexing/pthread_test.cpp
)

add_executable(chatroom_client
chapter9_io_multiplexing/chatroom_client.cpp
)

add_executable(chatroom_server
chapter9_io_multiplexing/chatroom_server.cpp
)

add_executable(ptus_test
chapter9_io_multiplexing/process_tcp_udp_simultaneously.cpp
)

add_executable(ues_test
chapter10_signal/unified_event_source.cpp
)

add_executable(sot
chapter10_signal/sigurg_oob_test.cpp
)

add_executable(stlt
chapter11_timer/sorted_timer_list_test.cpp   
)

add_executable(cnac
chapter11_timer/close_nonactivity_connection.cpp   
)
add_executable(hello_libevent
chapter12_libevent/hello_libevent.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVENT REQUIRED libevent)

target_include_directories(hello_libevent PRIVATE ${LIBEVENT_INCLUDE_DIRS})
target_link_libraries(hello_libevent PRIVATE ${LIBEVENT_LIBRARIES})

