cmake_minimum_required(VERSION 3.0)
project(service_testsuite VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB TEST_SRCS src/*.cpp)


include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/../src)
include_directories(${PROJECT_SOURCE_DIR}/../platform/src)

add_executable(service_testsuite ${TEST_SRCS} )

target_link_libraries(service_testsuite PUBLIC service_lib platform cppunit)

add_test(NAME service WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND service_testsuite -all)
