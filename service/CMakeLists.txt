cmake_minimum_required(VERSION 3.0)
project(service VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SRCS_G
        src/*.cpp
        src/rpc/*.cpp
        src/handlers/*.cpp
        src/handlers/json-rpc/*.cpp
        )

include_directories(
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../platform/src
)

link_directories(
    /usr/local/lib
)

add_executable(service ${SRCS_G})
add_library(service_lib ${SRCS_G})
add_subdirectory(testsuite)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/service.properties DESTINATION ${EXECUTABLE_OUTPUT_PATH})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/www/build/ DESTINATION ${EXECUTABLE_OUTPUT_PATH}/../var/www)

target_link_libraries(service platform ${CONAN_LIBS})



