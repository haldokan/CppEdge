cmake_minimum_required(VERSION 3.17)
project(cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp1 book/chp1/basics.cpp)
add_executable(cpp2 book/chp2/user_defined_types.cpp book/chp2/Vector.cpp book/chp2/Vector.h book/chp2/Vector2.cpp)
add_executable(cpp3 book/chp3/classes.cpp book/chp3/Complex.cpp book/chp3/headers/Complex.h)
