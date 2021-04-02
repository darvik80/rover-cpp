cmake_minimum_required(VERSION 3.0)
project(incubator VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

file(GLOB
        SRCS_G
        src/*.cpp
        src/module/*.cpp
        src/module/raspberry/*.cpp
        ${PROJECT_SOURCE_DIR}/../lib/platform-io/lib/shared/src/raspberry/*.cpp
)

include_directories(
        /usr/local/include
        /usr/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../platform/src
        ${PROJECT_SOURCE_DIR}/../lib/platform-io/lib/shared/src
)

add_definitions(-DPROFILE_RASPBERRY)

add_executable(incubator ${SRCS_G})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

target_link_libraries(incubator ${MAIN_LIBS})
