cmake_minimum_required(VERSION 3.12)
project(mqtt-example VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-type-limits -Wno-psabi")
endif ()

file(GLOB SRCS_G
        src/*.cpp
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

set(MQTT_EXAMPLE_MAIN_LIBS mqtt)

add_executable(${PROJECT_NAME} ${SRCS_G})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${MQTT_EXAMPLE_MAIN_LIBS})

