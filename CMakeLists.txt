cmake_minimum_required(VERSION 3.17)
project(cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp1 braintrain/newton/newton.cpp)
add_executable(cpp2 braintrain/einstein/einstein.cpp braintrain/einstein/Vector.cpp braintrain/einstein/headers/Vector.h braintrain/einstein/Vector_cpp20.cpp)
# note that this executable uses Vector.h and Vector.cpp from einstein so they must be included here so the linker can find & link them
add_executable(
        cpp3
        braintrain/pythagoras/pythagoras.cpp
        braintrain/pythagoras/Complex2.cpp
        braintrain/pythagoras/headers/Complex3.h
        braintrain/pythagoras/Vector_Container.cpp
        braintrain/pythagoras/headers/Container1.h
        braintrain/pythagoras/headers/Vector_Container.h
        braintrain/einstein/headers/Vector.h
        braintrain/einstein/Vector.cpp
        braintrain/pythagoras/Container_Factory.cpp braintrain/pythagoras/headers/Container_Factory.h)
add_executable(cpp4 braintrain/khwarizmi/khwarizmi.cpp braintrain/khwarizmi/Complex3.cpp braintrain/khwarizmi/headers/Complex3.h)
