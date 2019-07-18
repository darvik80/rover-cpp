cmake_minimum_required(VERSION 3.0)

project(platform VERSION 1.0)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

file(GLOB SRCS_G
        src/*.cpp
        src/json/*.cpp
        src/plugin/*.cpp
        src/rpc/*.cpp
        src/rpc/json/*.cpp
)

include_directories(
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/src
)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)

add_library(platform STATIC ${SRCS_G})

add_subdirectory(testsuite)

target_link_libraries(platform ${CONAN_LIBS})
