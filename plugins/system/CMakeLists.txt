cmake_minimum_required(VERSION 3.0)
project(system VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SRCS_G src/*.cpp)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../../platform/src
)

message(${PROJECT_SOURCE_DIR}/../platform/src)

add_library(system SHARED ${SRCS_G})

add_subdirectory(testsuite)

target_link_libraries(system platform ${CONAN_LIBS})

SET(CMAKE_LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../lib)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../lib)
