cmake_minimum_required(VERSION 3.0)
project(system VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SRCS_G
        src/*.cpp
        )

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/../platform/src)

add_library(system SHARED ${SRCS_G})

target_link_libraries(system PUBLIC
        platform
        ${CMAKE_THREAD_LIBS_INIT}
        -lPocoUtil
        -lPocoFoundation
        -lPocoNet
        -lPocoJSON
        )

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../lib)
