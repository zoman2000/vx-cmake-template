project(vx-cmake-template)
cmake_minimum_required(VERSION 3.10)

# recommended minimum standard
set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    if(VXPRO_COVERAGE)
        message(STATUS "*** coverage for lib")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage")
    endif()
endif()

# include_directories(... some include directory ...)
find_package(Boost 1.65.1 COMPONENTS system REQUIRED)

file(GLOB SOURCES
    ../src/*.h
    ../src/*.cpp
)

add_library(vx-cmake-template_a ${SOURCES})

add_library(vx-cmake-template_lib SHARED ${SOURCES})

