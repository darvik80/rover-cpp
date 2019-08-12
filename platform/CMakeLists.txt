cmake_minimum_required(VERSION 3.2)

project(platform VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

find_package(nlohmann_json 3.2.0 REQUIRED)

file(GLOB SRCS_G
        src/json/*.cpp
        src/plugin/*.cpp
        src/rpc/*.cpp
        src/rpc/json/*.cpp
)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib)

add_library(platform STATIC ${SRCS_G})

add_subdirectory(testsuite)

target_link_libraries(platform ${MAIN_LIBS} ${PLATFORM_SPECIFIC_LIBS} nlohmann_json::nlohmann_json)
