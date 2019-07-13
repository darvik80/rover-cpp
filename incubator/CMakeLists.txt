cmake_minimum_required(VERSION 3.0)
project(incubator VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

file(GLOB SRCS_G
        src/*.cpp
        src/http/*.cpp
        src/properties/*.cpp
        src/subsystem/*.cpp
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../platform/src
)

add_executable(incubator ${SRCS_G})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

target_link_libraries(incubator ${CONAN_LIBS})

set(PROPERTIES_DIR ${EXECUTABLE_OUTPUT_PATH}/../etc)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/settings.json DESTINATION ${PROPERTIES_DIR})

file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/../var/logs)
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/../var/tmp)



