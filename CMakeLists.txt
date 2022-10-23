# ############################################################
# Sample cmake project
# to enable CPPUNIT set(USE_CPPUNIT 1), default is disabled
# to enable GTEST set(USE_GTEST 1), default is enabled
# ############################################################
project(vx-cmake-template)

cmake_minimum_required(VERSION 3.10)

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same.")
endif()

# # this variable will be available in the source using #if defined(MY_VARIABLE) or just #ifdef MY_VARIABLE
add_compile_options(-DMY_VARIABLE)

# recommended minimum standard
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -O2")

# installation path
set(PROJECT_INSTALL_DIR "/opt/sample/vx-cmake-template")

# libraties will be built (shared and static), main could be linked with either one of those
set(USING_LIB 1)

# if enabled "cppunit" target will be built (cppunit has to be installed), this is a very old library
set(USING_CPPUNIT 1)

# if enabled "getest" target will be built (gtest has to be installed)
set(USING_GTEST 1)

# comment-out to use shared library
# set(LINK_TYPE static)

# ##########################
# ## coverage settings #####
# ##########################
if(DEFINED ENV{VXPRO_COVERAGE})
    set(VXPRO_COVERAGE true)
endif()

# coverage is disabled for MinSizeRel target
if(NOT CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    if(VXPRO_COVERAGE)
        message(STATUS "*** coverage for main")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage")
        SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_COVERAGE} --coverage")
        SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_COVERAGE} --coverage")
    endif()
endif()

# ##########################
# ## CONFIG params #########
# ##########################
include(./GitVersion.cmake)

if(PROJECT_GIT_TAG)
    add_compile_options(-DPROJECT_GIT_TAG="${PROJECT_GIT_TAG}")
endif()

message(STATUS "Using version: ${PROJECT_GIT_TAG}")

# ##########################
# ## packages          #####
# ##########################
find_package(Boost 1.65.1 COMPONENTS program_options system thread filesystem serialization REQUIRED)
find_package(PkgConfig)

if(USING_GTEST)
    pkg_search_module(GTEST QUIET gtest_main)

    if(NOT GTEST_LDFLAGS)
        message(STATUS, "*** gtest library is not installed. Use package manager to install it")
    endif()
endif()

if(USING_CPPUNIT)
    pkg_search_module(CPPUNIT QUIET cppunit)

    if(NOT CPPUNIT_LDFLAGS)
        message(STATUS, "*** cppunit library is not installed. Use package manager to install it")
    endif()
endif()

find_package(Doxygen OPTIONAL_COMPONENTS dot)

if(DOXYGEN_FOUND)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
    doxygen_add_docs(doc
        ${CMAKE_SOURCE_DIR}
    )
endif(DOXYGEN_FOUND)

include_directories(src)

# Those files will be put to lib if necessary
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

file(GLOB SOURCES_GTEST
    gtest/*.h
    gtest/*.cpp
)

if(USING_LIB)
    message(STATUS "*** ADD project library ***")
    add_subdirectory(lib)
endif()

if(USING_LIB)
    add_executable(vx-cmake-template main.cpp)

    if(LINK_TYPE STREQUAL static)
        target_link_libraries(vx-cmake-template PRIVATE vx-cmake-template_a)
    else()
        target_link_libraries(vx-cmake-template PRIVATE vx-cmake-template_lib)
    endif()
else()
    add_executable(vx-cmake-template main.cpp ${SOURCES})
endif()

target_link_libraries(vx-cmake-template PRIVATE ${Boost_LIBRARIES})
target_link_libraries(vx-cmake-template PRIVATE pthread)
target_include_directories(vx-cmake-template PRIVATE ${Boost_INCLUDE_DIR})

# Strip binary for release builds
if(CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_STRIP} ${PROJECT_NAME})
endif()

# #################################
# #### test files (CPPUNIT) #######
# #################################
if(CPPUNIT_LDFLAGS)
    if(NOT CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
        if(USING_LIB)
            add_executable(vx-cmake-template-test main_test.cpp ${SOURCES_TEST})

            if(LINK_TYPE STREQUAL static)
                target_link_libraries(vx-cmake-template-test PRIVATE vx-cmake-template_a)
            else()
                target_link_libraries(vx-cmake-template-test PRIVATE vx-cmake-template_lib)
            endif()
        else()
            add_executable(vx-cmake-template-test main_test.cpp ${SOURCES_TEST} ${SOURCES})
        endif()

        target_link_libraries(vx-cmake-template-test PRIVATE ${CPPUNIT_LDFLAGS})
        target_link_libraries(vx-cmake-template-test PRIVATE ${Boost_LIBRARIES})
        target_link_libraries(vx-cmake-template-test PRIVATE pthread)
        target_include_directories(vx-cmake-template-test PRIVATE ${Boost_INCLUDE_DIR})
    endif()
endif()

# #################################
# #### test files (GTEST) #########
# #################################
if(GTEST_LDFLAGS)
    if(NOT CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
        if(USING_LIB)
            add_executable(vx-cmake-template-gtest main_gtest.cpp ${SOURCES_GTEST})
            target_link_libraries(vx-cmake-template-gtest PRIVATE vx-cmake-template_a)
        else()
            add_executable(vx-cmake-template-gtest main_gtest.cpp ${SOURCES_GTEST} ${SOURCES})
        endif()

        target_link_libraries(vx-cmake-template-gtest PRIVATE ${Boost_LIBRARIES})
        target_link_libraries(vx-cmake-template-gtest PRIVATE pthread)
        target_link_libraries(vx-cmake-template-gtest PRIVATE ${GTEST_LDFLAGS})
        target_compile_options(vx-cmake-template-gtest PUBLIC ${GTEST_CFLAGS})
        target_include_directories(vx-cmake-template-gtest PRIVATE ${Boost_INCLUDE_DIR})
    endif()
endif()

# #######################
# #### static files #####
# #######################
get_filename_component(BINOUT ${PROJECT_BINARY_DIR} NAME)
message(STATUS "binary folder: " ${BINOUT})

message(STATUS "*** copy configuration files to build folder ***")

# #configure_file(file.one file.one COPYONLY)

# #######################
# #### INSTALL      #####
# #######################
if(CMAKE_BUILD_TYPE STREQUAL MinSizeRel OR CMAKE_BUILD_TYPE STREQUAL Release)
    message(STATUS "*** generating change_log.txt file")
    execute_process(COMMAND git log --date=format-local:%a\ %b\ %d\ %Y -10 --pretty=\*\ %ad\ %an\ \<%ae\>\ -\ %\(describe:tags=true\)%n\ \ \%s OUTPUT_FILE "change_log.txt") # '--pretty=format:%cs%s'

    SET(PACKAGE_MAINTAINER "Alex Smith")
    SET(PACKAGE_DESCRIPTION "Sample project using cmake, cppunit, gtest, lcov")

    SET(CPACK_GENERATOR "RPM")
    SET(CPACK_SYSTEM_NAME linux)

    if(EXISTS /etc/os-release)
        set(OSREL "")

        file(STRINGS /etc/os-release OSREL REGEX "^ID=.*")

        if(OSREL STREQUAL "")
            message(FATAL_ERROR "/etc/os-release ID is not set")
        endif()

        # GET operating system ID
        string(REGEX MATCH "^ID=\"([a-z-]+)\"" OSM ${OSREL})
        set(OSM ${CMAKE_MATCH_1})

        if("${OSM}" STREQUAL "")
            string(REGEX MATCH "^ID=([a-z-]+)" OSM ${OSREL})
            set(OSM ${CMAKE_MATCH_1})
        endif()

        if(OSM STREQUAL "ubuntu")
            SET(CPACK_GENERATOR "DEB")
            SET(CPACK_SYSTEM_NAME ubuntu)
        endif()

        if(OSM STREQUAL "debian")
            SET(CPACK_GENERATOR "DEB")
            SET(CPACK_SYSTEM_NAME debian)
        endif()
    endif()

    install(TARGETS vx-cmake-template RUNTIME DESTINATION "${PROJECT_INSTALL_DIR}")

    if(USING_LIB)
        set(CMAKE_INSTALL_LIBDIR "local/lib64")
        install(TARGETS vx-cmake-template_a RUNTIME)
        install(TARGETS vx-cmake-template_lib RUNTIME)
    endif()

    if(EXISTS ${CMAKE_BINARY_DIR}/change_log.txt)
        message(STATUS "*** set change log file")
        SET(CPACK_RPM_CHANGELOG_FILE ${CMAKE_BINARY_DIR}/change_log.txt)
    else()
        message(STATUS "*** WARNING change log file ${CMAKE_BINARY_DIR}/change_log.txt does not exist")
    endif()

    SET(CPACK_RPM_PACKAGE_MAINTAINER ${PACKAGE_MAINTAINER})
    SET(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/vx-cmake-template.description.txt")
    SET(CPACK_PACKAGE_VENDOR "VxPRO")
    SET(CPACK_RPM_PACKAGE_SUMMARY ${PACKAGE_DESCRIPTION})

    SET(CPACK_PACKAGE_VERSION_MAJOR ${VXPRO_VERSION_MAJOR})
    SET(CPACK_PACKAGE_VERSION_MINOR ${VXPRO_VERSION_MINOR})
    SET(CPACK_PACKAGE_VERSION_PATCH ${VXPRO_VERSION_PATCH})

    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER ${PACKAGE_MAINTAINER})
    SET(CPACK_DEBIAN_PACKAGE_NAME "${PROJECT_NAME}")
    SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${PACKAGE_DESCRIPTION})

    include(CPack)
endif()
