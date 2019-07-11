cmake_minimum_required(VERSION 3.0)
project(incubator VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SRCS_G
        src/*.cpp
        src/http/*.cpp
        src/properties/*.cpp
        )

file(GLOB LIB_SRCS_G
        src/http/*.cpp
        )

include_directories(
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../platform/src
)

add_executable(incubator ${SRCS_G})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

target_link_libraries(incubator ${CONAN_LIBS})



