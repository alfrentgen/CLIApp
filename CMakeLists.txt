cmake_minimum_required(VERSION 3.27)
project(CLIApp)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB SRC *.h *.cpp)

add_library(app_lib STATIC ${SRC})
target_include_directories(app_lib PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(CLIApp main.cpp)
target_link_libraries(CLIApp app_lib)

option(ENABLE_GTESTS "Enables googletests" ON)

MESSAGE(STATUS ${ENABLE_GTESTS})

if (${ENABLE_GTESTS})
    set(GTEST_SRC_DIR "${CMAKE_SOURCE_DIR}/googletest")
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY    https://github.com/google/googletest.git
            GIT_TAG           main
            SOURCE_DIR        ${GTEST_SRC_DIR}
    )
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(gtests)
endif()

