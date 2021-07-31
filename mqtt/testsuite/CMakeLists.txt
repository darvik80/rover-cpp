cmake_minimum_required(VERSION 3.12)
project(mqtt-testsuite VERSION 1.0)

file(GLOB TEST_SRCS
        src/*.cpp
        ../src/mqtt/*.cpp
        ../src/mqtt/Decoder.cpp
        ../src/mqtt/Encoder.cpp
        ../src/mqtt/TopicFilter.cpp
        ../src/mqtt/message/*.cpp
)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../src
)

enable_testing()

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} ${TEST_SRCS})

target_link_libraries(${PROJECT_NAME} PUBLIC
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        pthread
        )

add_test(NAME ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMAND ${PROJECT_NAME})