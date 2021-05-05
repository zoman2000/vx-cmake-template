project(vx-cmake-template)

cmake_minimum_required(VERSION 3.10)
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same.")
endif()

add_compile_options(-DMY_VARIABLE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-O2 -march=native")
set(VXPRO_DIR "/opt/vxpro/vx-cmake-template")
set(USING_LIB 1)

find_package(Boost 1.61.0 COMPONENTS program_options system thread filesystem serialization REQUIRED)

include_directories(src)

#Those files will be put to lib if necessary
file(GLOB SOURCES
    src/*.cpp
)

file(GLOB SQL_SCRIPTS
    sql/*.sql
)

file(GLOB SOURCES_TEST
    test/*.h
    test/*.cpp
)

if(USING_LIB)
    message(STATUS "*** ADD project library ***")
    add_subdirectory(lib)
endif()


if(USING_LIB)
    add_executable(vx-cmake-template main.cpp)
    target_link_libraries(vx-cmake-template PRIVATE vx-cmake-template_a)
else()
    add_executable(vx-cmake-template main.cpp ${SOURCES})
endif()
target_link_libraries(vx-cmake-template PRIVATE ${Boost_LIBRARIES})
target_link_libraries(vx-cmake-template PRIVATE db_cxx)
target_link_libraries(vx-cmake-template PRIVATE crypt owcrypt ssl crypto uuid)
target_link_libraries(vx-cmake-template PRIVATE pthread)
target_include_directories(vx-cmake-template PRIVATE ${Boost_INCLUDE_DIR})
# Strip binary for release builds
if (CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_STRIP} ${PROJECT_NAME})
endif ()

########################
##### test files #######
########################

if (NOT CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    if(USING_LIB)
        add_executable(vx-cmake-template-test main_test.cpp ${SOURCES_TEST})
        target_link_libraries(vx-cmake-template-test PRIVATE vx-cmake-template_a)
    else()
        add_executable(vx-cmake-template-test main_test.cpp ${SOURCES_TEST} ${SOURCES})
    endif()
    target_link_libraries(vx-cmake-template-test PRIVATE cppunit)
    target_link_libraries(vx-cmake-template-test PRIVATE ${Boost_LIBRARIES})
    target_link_libraries(vx-cmake-template-test PRIVATE db_cxx)
    target_link_libraries(vx-cmake-template-test PRIVATE crypt owcrypt ssl crypto uuid)
    target_link_libraries(vx-cmake-template-test PRIVATE pthread)
    target_include_directories(vx-cmake-template-test PRIVATE ${Boost_INCLUDE_DIR})
endif()


########################
##### static files #####
########################

get_filename_component(BINOUT ${PROJECT_BINARY_DIR} NAME)
message(STATUS "binary folder: " ${BINOUT})

message(STATUS "*** copy configuration files to build folder ***")
##configure_file(file.one file.one COPYONLY)

########################
##### INSTALL      #####
########################

if (CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    install(TARGETS vx-cmake-template RUNTIME DESTINATION "${VXPRO_DIR}")

#    SET(CPACK_GENERATOR "RPM")
#    SET(CPACK_RPM_PACKAGE_MAINTAINER "Alex Smith")
#    SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/vx-cmake-template.description.txt")
#    SET(CPACK_PACKAGE_VENDOR "VxPRO")
#    SET(CPACK_PACKAGE_VERSION_MAJOR 0)
#    SET(CPACK_PACKAGE_VERSION_MINOR 1)
#    SET(CPACK_PACKAGE_VERSION_PATCH 1)
#    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/my_project.spec.in" "${CMAKE_CURRENT_BINARY_DIR}/my_project.spec" @ONLY IMMEDIATE)
#    set(CPACK_RPM_USER_BINARY_SPECFILE "${CMAKE_CURRENT_BINARY_DIR}/my_project.spec")
#    include(CPack)

endif()