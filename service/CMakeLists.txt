cmake_minimum_required(VERSION 3.0)
project(service VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SRCS_G
        src/*.cpp
        src/rpc/*.cpp
        src/handlers/*.cpp
        src/handlers/json-rpc/*.cpp
        )

file(GLOB LIB_SRCS_G
        src/rpc/*.cpp
        src/handlers/*.cpp
        src/handlers/json-rpc/*.cpp
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../platform/src
)

add_executable(service ${SRCS_G})
add_library(service_lib ${LIB_SRCS_G})
add_subdirectory(testsuite)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
set(PROPERTIES_DIR ${EXECUTABLE_OUTPUT_PATH}/../etc)
set(PLUGINS_DIR ${EXECUTABLE_OUTPUT_PATH}/../lib)

file(MAKE_DIRECTORY ${PROPERTIES_DIR})
file(MAKE_DIRECTORY ${PLUGINS_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/service.properties DESTINATION ${PROPERTIES_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/www/build/ DESTINATION ${EXECUTABLE_OUTPUT_PATH}/../var/www)

target_link_libraries(service platform ${CONAN_LIBS})



