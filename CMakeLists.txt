cmake_minimum_required(VERSION 3.17)
project(cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp1 chp1/basics.cpp)
add_executable(cpp2 chp2/user_defined_types.cpp chp2/Vector.cpp chp2/Vector.h chp2/Vector2.cpp)