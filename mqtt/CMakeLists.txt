cmake_minimum_required(VERSION 3.12)
project(mqtt VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-type-limits -Wno-psabi")
endif ()

file(GLOB SRCS_G
        src/mqtt/*.cpp
        src/mqtt/message/*.cpp
        src/mqtt/logging/*.cpp
        src/mqtt/event/*.cpp
        src/mqtt/command/*.cpp
        )

include_directories(
        /usr/include
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../core-utils/logging/src
        ${PROJECT_SOURCE_DIR}/../core-utils/event-manager/src
)

add_definitions(-DBOOST_LOG_DYN_LINK)

find_package(Boost REQUIRED COMPONENTS system program_options)
find_package(Threads)

link_directories(
        /usr/local/lib
)

set(MQTT_MAIN_LIBS logging event-manager)

add_library(${PROJECT_NAME} STATIC ${SRCS_G})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${MQTT_MAIN_LIBS})

add_subdirectory(testsuite)
add_subdirectory(example)