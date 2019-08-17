cmake_minimum_required(VERSION 3.0)
project(incubator VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

file(GLOB SRCS_G
        src/*.cpp
        src/http/*.cpp
        src/http/json-rpc/*.cpp
        src/properties/*.cpp
        src/subsystem/*.cpp
        )

include_directories(
        /usr/local/include
        /usr/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../platform/src
)

find_package(nlohmann_json 3.7.0 REQUIRED)

add_executable(incubator ${SRCS_G})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

target_link_libraries(incubator ${CONAN_LIBS} nlohmann_json::nlohmann_json)
