project(tinyhttpd)


add_executable(my_test
mystudy/mytest.cpp
)

add_executable(fork_test
mystudy/fork_test.cpp
)

add_executable(pipe_test
mystudy/pipe_test.cpp
)

add_executable(httpd
httpd.c
)

add_executable(backlog_test
mystudy/backlog.cpp
)

add_executable(accept_test
mystudy/accept_test.cpp
)