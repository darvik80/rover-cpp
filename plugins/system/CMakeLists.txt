cmake_minimum_required(VERSION 3.0)
project(system VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SRCS_G src/*.cpp)

include_directories(
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../platform/src
)

link_directories(
        /usr/local/lib
)


add_library(system SHARED ${SRCS_G})
add_subdirectory(testsuite)

target_link_libraries(system PUBLIC
        platform
        ${CMAKE_THREAD_LIBS_INIT}
        -lPocoUtil
        -lPocoFoundation
        -lPocoNet
        -lPocoJSON
        )

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../lib)
