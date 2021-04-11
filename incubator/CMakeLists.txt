cmake_minimum_required(VERSION 3.0)
project(incubator VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

add_definitions(-DRASPBERRY_ARCH -DSUPPORT_ETHERNET)

file(GLOB  SRCS_G
        src/*.cpp
        src/module/*.cpp
        src/properties/*.cpp
        src/properties/source/*.cpp
        src/module/raspberry/*.cpp
        ${PROJECT_SOURCE_DIR}/../lib/platform-io/lib/core/src/net/*.cpp
        src/net/*.cpp
        src/serial/*.cpp
        src/serial/os/*.cc
)

include_directories(
        /usr/local/include
        /usr/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../platform/src
        ${PROJECT_SOURCE_DIR}/../lib/platform-io/lib/core/src
        resources_manager/src
)

set(RESOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)

set(TARGET_FORMAT "elf64-x86-64")
set(TARGET_ARCHITECTURE "i386:x86-64")

add_subdirectory(resources_manager)
add_executable(${PROJECT_NAME} ${SRCS_G} ${RESOURCE_DIRECTORY}/settings.json)

set(RESOURCE_FILES ${RESOURCE_DIRECTORY}/settings.json)

set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE
        MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
        RESOURCE "${RESOURCE_FILES}")

target_link_libraries(${PROJECT_NAME} ${MAIN_LIBS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
