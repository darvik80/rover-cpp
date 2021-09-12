cmake_minimum_required(VERSION 3.12)
project(zeromq)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCS_G src/zeromq/*.cpp src/zeromq/raspberry/*.cpp)

set(EVENT_MAIN_LIBS logging event-manager)
add_definitions(-DRASPBERRY_ARCH)

include_directories(
        src
)

add_library(${PROJECT_NAME} ${SRCS_G})
target_link_libraries(${PROJECT_NAME} ${EVENT_MAIN_LIBS})

add_subdirectory(testsuite)