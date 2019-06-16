cmake_minimum_required(VERSION 3.0)
project(platform VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SRCS_G
        src/json/*.cpp
        )

include_directories(${PROJECT_SOURCE_DIR}/src)

add_library(platform SHARED ${SRCS_G})

add_subdirectory(testsuite)

target_link_libraries(platform PUBLIC
        ${CMAKE_THREAD_LIBS_INIT}
        -lPocoUtil
        -lPocoFoundation
        -lPocoNet
        -lPocoJSON
        )

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib)
