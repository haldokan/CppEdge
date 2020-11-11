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
add_executable(
        descartes
        braintrain/descartes/descartes.cpp)
add_executable(
        plato
        braintrain/plato/plato.cpp)
add_executable(
        darwin
        braintrain/darwin/darwin.cpp)
add_executable(
        feynman
        braintrain/feynman/feynman.cpp)
add_executable(
        kubernetes
        braintrain/kubernetes/kubernetes.cpp)
add_executable(
        pascal
        braintrain/pascal/pascal.cpp braintrain/pascal/EnglishDictionary.cpp braintrain/pascal/headers/EnglishDictionary.h)
add_executable(
        faraday
        braintrain/faraday/faraday.cpp)

find_package(Boost 1.74.0 COMPONENTS filesystem container date_time system thread REQUIRED)
add_executable(
        boostx
        braintrain/boost/boostx.cpp braintrain/boostdi/intro/boostdi.cpp)
target_link_libraries(boostx Boost::filesystem Boost::filesystem Boost::container Boost::date_time Boost::system Boost::thread)

add_executable(
        boostdi-intro
        braintrain/boostdi/intro/boostdi.cpp)
add_executable(
        boostdi-bindings1
        braintrain/boostdi/bindings/bindings1.cpp)
add_executable(
        boostdi-bindings2
        braintrain/boostdi/bindings/bindings2.cpp)

