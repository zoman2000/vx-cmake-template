cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(vx-cmake-template)
#include_directories(... some include directory ...)

find_package(Boost 1.61.0 COMPONENTS system REQUIRED)

file(GLOB SOURCES
    ../src/*.h
    ../src/*.cpp
)

add_library(vx-cmake-template_a ${SOURCES})
