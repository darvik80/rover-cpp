cmake_minimum_required(VERSION 3.0)
project(service VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SRCS_G
        src/*.cpp
        src/handlers/*.cpp
        src/handlers/json-rpc/*.cpp
        )

include_directories(${PROJECT_SOURCE_DIR}/../platform/src)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(service ${SRCS_G})

target_link_libraries(service PUBLIC
        platform
        ${CMAKE_THREAD_LIBS_INIT}
        -lPocoUtil
        -lPocoFoundation
        -lPocoNet
        -lPocoJSON
)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
