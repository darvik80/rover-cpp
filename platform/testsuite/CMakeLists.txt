cmake_minimum_required(VERSION 3.0)
project(platform_testsuite VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB TEST_SRCS src/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/../src)

add_executable(platform_testsuite ${TEST_SRCS} )

target_link_libraries(platform_testsuite PUBLIC -lcppunit platform)

add_test(NAME platform WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND platform_testsuite -all)
