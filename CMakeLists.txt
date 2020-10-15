cmake_minimum_required(VERSION 3.17)
project(cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp1 book/chp1/driver1.cpp)
add_executable(cpp2 book/chp2/driver2.cpp book/chp2/Vector.cpp book/chp2/Vector.h book/chp2/Vector_cpp20.cpp)
# note that this executable uses Vector.h and Vector.cpp from chp2 so they must be included here so the linker can find & link them
add_executable(
        cpp3
        book/chp3/driver3.cpp
        book/chp3/Complex.cpp
        book/chp3/headers/Complex.h
        book/chp3/Vector_Container.cpp
        book/chp3/headers/Container1.h
        book/chp3/headers/Vector_Container.h
        book/chp2/Vector.h
        book/chp2/Vector.cpp
        book/chp3/Container_Factory.cpp book/chp3/headers/Container_Factory.h)
