set(VXPRO_VERSION_MAJOR 0)
set(VXPRO_VERSION_MINOR 0)
set(VXPRO_VERSION_PATCH 1)
set(PROJECT_GIT_TAG "0.0.1")

message(STATUS "*** INFO: GitVersion.cmake" )

### check we have PROJECT_TAG??? variables set by build script
if(DEFINED ENV{PROJECT_TAG})
    message(STATUS "*** INFO: set project version from PROJECT_TAG variable: ${PROJECT_TAG}" )
    set(PROJECT_GIT_TAG $ENV{PROJECT_TAG})
else()
    message(STATUS "*** INFO: set project version using GIT tags" )
    execute_process(COMMAND git tag -l --contains HEAD 
        OUTPUT_VARIABLE PROJECT_GIT_TAG 
        OUTPUT_STRIP_TRAILING_WHITESPACE
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )

    if(NOT PROJECT_GIT_TAG)
        execute_process(COMMAND git rev-list --tags --max-count=1 
            OUTPUT_VARIABLE PROJECT_GIT_COMMIT 
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
        execute_process(COMMAND git describe --tags ${PROJECT_GIT_COMMIT} 
            OUTPUT_VARIABLE PROJECT_GIT_TAG 
            ERROR_QUIET 
            OUTPUT_STRIP_TRAILING_WHITESPACE
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
    endif()
endif()

if(PROJECT_GIT_TAG)
    string(REPLACE "." ";" TAG_LIST ${PROJECT_GIT_TAG})

    message(STATUS "*** generating change_log.txt file")
    execute_process(COMMAND git log --date=format-local:%a\ %b\ %d\ %Y -10 --pretty=\*\ %ad\ %an\ \<%ae\>\ -\ %\(describe:tags=true\)%n\ \ \%s OUTPUT_FILE "change_log.txt") # '--pretty=format:%cs%s'

    list(LENGTH TAG_LIST len)
    if(len EQUAL 3)
        list(GET TAG_LIST 0 VXPRO_VERSION_MAJOR)
        list(GET TAG_LIST 1 VXPRO_VERSION_MINOR)
        list(GET TAG_LIST 2 VXPRO_VERSION_PATCH)
    else()
        set(PROJECT_GIT_TAG "0.0.1")            
    endif()
endif()

message(STATUS "Git version is: ${VXPRO_VERSION_MAJOR}.${VXPRO_VERSION_MINOR}.${VXPRO_VERSION_PATCH}")

