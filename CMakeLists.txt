cmake_minimum_required(VERSION 3.8)
project(Polynomial_Root_Finding_pGA)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        World/Individual.cpp
        World/Individual.h
        World/Population.cpp
        World/Population.h
        main.cpp
        World.h)

add_executable(Polynomial_Root_Finding_pGA ${SOURCE_FILES})