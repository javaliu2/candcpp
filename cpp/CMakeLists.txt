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