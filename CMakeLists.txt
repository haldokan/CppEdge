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

add_executable(
        boostx
        braintrain/boost/Gtest.cpp)

target_link_libraries(boostx
        gtest_main # Links to Google Test library and its main function
        pthread)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
)
# Fetch the content and add it to the build. This makes GTest targets available.
FetchContent_MakeAvailable(googletest)

add_executable(
        boostdi-intro
        braintrain/boostdi/intro/boostdi.cpp)
add_executable(
        boostdi-bindings1
        braintrain/boostdi/bindings/bindings1.cpp
        braintrain/boost/hash_map.cpp
        braintrain/boost/hash_map.h
        braintrain/boost/UniquePointer.cpp
        braintrain/boost/SharedPointer.cpp
        braintrain/boost/GenericLambda.cpp
)
add_executable(
        boostdi-bindings2
        braintrain/boostdi/bindings/bindings2.cpp)
add_executable(
        boostdi-bindings3
        braintrain/boostdi/bindings/bindings3.cpp)
add_executable(
        boostdi-bindings4
        braintrain/boostdi/bindings/bindings4.cpp)
add_executable(
        boostdi-bindings5
        braintrain/boostdi/bindings/bindings5.cpp)

