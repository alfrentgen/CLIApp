cmake_minimum_required(VERSION 3.27)
project(CLIApp)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB SRC *.h *.cpp)

add_executable(CLIApp ${SRC})
