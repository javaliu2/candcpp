cmake_minimum_required(VERSION 3.10)

project(boost-study)

set(CMAKE_CXX_STANDARD 11)

# find_package(Boost REQUIRED COMPONENTS asio system)
include_directories(/usr/include/)
link_directories(/usr/lib/)


    #include_directories(${Boost_INCLUDE_DIRS})
    #MESSAGE(STATUS "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}")
    #MESSAGE(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
    #MESSAGE(STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}")
    add_executable(echo_server ${CMAKE_SOURCE_DIR}/src/echo_server.cpp)
    add_executable(echo_client ${CMAKE_SOURCE_DIR}/src/echo_client.cpp)
    target_link_libraries(echo_server ${Boost_LIBRARIES})
    target_link_libraries(echo_client ${Boost_LIBRARIES})