project(gtests)

FILE(GLOB SRC *.h *.cpp)

MESSAGE(STATUS "!")
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main app_lib)