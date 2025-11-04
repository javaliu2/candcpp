cmake_minimum_required(VERSION 3.22)
project(CPP)

include_directories(./header ./src)

add_executable(stu 
src/student.cpp entry/const_object.cpp)

add_executable(inherit
src/animal.cpp src/cat.cpp entry/inheritation.cpp
)

add_executable(poly
entry/polymorphism.cpp
)

add_executable(poly2
entry/poly2.cpp
)

add_executable(copy_ref_value
entry/copy_ref_value.cpp
)

add_executable(type_cast
src/fraction.cpp src/student.cpp entry/type_cast.cpp)

add_executable(vector_test
header/vector.h entry/vector_test.cpp src/student.cpp
)

add_executable(1_create_thread
thread/1_create_thread.cpp
)

add_executable(2_variable_pass
thread/2_variable_pass.cpp
)

add_executable(stack_and_heap
src/stack_and_heap.cpp
)

add_executable(useless_main
header/useless.h entry/useless_main.cpp src/useless.cpp
)

add_executable(3_mutex
thread/3_mutex.cpp
)
add_executable(4_single_instance
thread/4_single_instance.cpp)

add_executable(5_producer_consumer
thread/5_producer_consumer.cpp)

add_executable(reference
src/reference.cpp)

add_executable(vector_emplace
src/vector_emplace.cpp)

add_executable(6_thread_pool
thread/6_thread_pool.cpp)