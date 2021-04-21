cmake_minimum_required(VERSION 3.0)
project(incubator VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

add_definitions(-DRASPBERRY_ARCH -DSUPPORT_ETHERNET)

file(GLOB SRCS_G
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
)

set(RESOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)

if (APPLE)
    message("Apple incubator build")
    file(GLOB SRCS_OS
            src/resources/ResourceManager_apple.cpp
            )
    file(GLOB RESOURCE_G
            ${RESOURCE_DIRECTORY}/*.*
            )

    set(RESOURCE_FILES ${RESOURCE_G})

    add_executable(${PROJECT_NAME} ${SRCS_G} ${SRCS_OS} ${RESOURCE_G})

    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE
            MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget
            RESOURCE "${RESOURCE_FILES}"
            )
else ()
    set(TARGET_FORMAT "elf64-littleaarch64")
    set(TARGET_ARCHITECTURE "aarch64")

    include(CMakeLists_linux.txt)

    add_executable(${PROJECT_NAME} ${SRCS_G} ${RES_SRC_FILE} ${RES_EMBED_FILES})
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
endif ()

target_link_libraries(${PROJECT_NAME} ${MAIN_LIBS})