cmake_minimum_required(VERSION 3.17)
project(braintrain)

set(CMAKE_CXX_STANDARD 20)

add_executable(newton braintrain/newton/newton.cpp)
add_executable(
        einstein
        braintrain/einstein/einstein.cpp
        braintrain/einstein/Vector.cpp
        braintrain/einstein/headers/Vector.h
        braintrain/einstein/Vector_cpp20.cpp)
# note that this executable uses Vector.h and Vector.cpp from einstein so they must be included here so the linker can find & link them
add_executable(
        pythagoras
        braintrain/pythagoras/pythagoras.cpp
        braintrain/pythagoras/headers/Complex3.h
        braintrain/pythagoras/Vector_Container.cpp
        braintrain/pythagoras/headers/Container1.h
        braintrain/pythagoras/headers/Vector_Container.h
        braintrain/einstein/headers/Vector.h
        braintrain/einstein/Vector.cpp
        braintrain/pythagoras/Container_Factory.cpp
        braintrain/pythagoras/headers/Container_Factory.h
        braintrain/pythagoras/Complex3.cpp)
add_executable(
        khwarizmi
        braintrain/khwarizmi/khwarizmi.cpp
        braintrain/khwarizmi/Complex3.cpp
        braintrain/khwarizmi/headers/Complex3.h
        braintrain/khwarizmi/Vector3.cpp
        braintrain/khwarizmi/headers/Vector3.h)
add_executable(
        riemann
        braintrain/riemann/headers/TVector.h
        braintrain/riemann/riemann.cpp
        braintrain/riemann/headers/Vehicle.h
        braintrain/riemann/headers/Truck.h
        braintrain/riemann/headers/Sedan.h
        braintrain/riemann/Sedan.cpp
        braintrain/riemann/Truck.cpp)