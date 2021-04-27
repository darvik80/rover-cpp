cmake_minimum_required(VERSION 3.0)
project(incubator VERSION 1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

add_definitions(-DRASPBERRY_ARCH -DSUPPORT_ETHERNET)

file(GLOB SRCS_G
        src/*.cpp
        src/logging/*.cpp
        src/module/*.cpp
        src/properties/*.cpp
        src/properties/source/*.cpp
        src/module/raspberry/*.cpp
        src/net/*.cpp
        src/net/http/*.cpp
        src/net/http/rpc/*.cpp
        src/net/http/rpc/json/*.cpp
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
set(WWW_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/www)

add_definitions(-DBOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED -DDBOOST_STACKTRACE_USE_ADDR2LINE)
if (APPLE)
    message("Apple incubator build")

    function(resource VAR SOURCE_PATH DESTINATION PATTERN)
        file(GLOB_RECURSE _LIST CONFIGURE_DEPENDS ${SOURCE_PATH}/${PATTERN})
        foreach (RESOURCE ${_LIST})
            get_filename_component(_PARENT ${RESOURCE} DIRECTORY)
            if (${_PARENT} STREQUAL ${SOURCE_PATH})
                set(_DESTINATION ${DESTINATION})
            else ()
                file(RELATIVE_PATH _DESTINATION ${SOURCE_PATH} ${_PARENT})
                set(_DESTINATION ${DESTINATION}/${_DESTINATION})
            endif ()
            set_property(SOURCE ${RESOURCE} PROPERTY MACOSX_PACKAGE_LOCATION ${_DESTINATION})
            message(${RESOURCE})
        endforeach (RESOURCE)
        set(${VAR} ${_LIST} PARENT_SCOPE)
    endfunction()

    resource(RESOURCES ${RESOURCE_DIRECTORY} Resources/ *.*)
    resource(HTML ${WWW_DIRECTORY}/build/ Resources/www *.*)
    resource(HTML_JS ${WWW_DIRECTORY}/build/static/js Resources/www/static/js *.js)
    resource(HTML_CSS ${WWW_DIRECTORY}/build/static/js Resources/www/static/css *.css)

    set(RESOURCES_G
            ${RESOURCES}
            ${HTML}
            ${HTML_JS}
            ${HTML_CSS}
            )

    file(GLOB SRCS_OS
            src/resources/ResourceManager_apple.cpp
            )

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRCS_G} ${SRCS_OS} ${RESOURCES_G})
else ()
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
    set(PROPERTIES_DIR ${EXECUTABLE_OUTPUT_PATH}/../etc)
    set(PLUGINS_DIR ${EXECUTABLE_OUTPUT_PATH}/../lib)
    set(VAR_DIR ${EXECUTABLE_OUTPUT_PATH}/../var)

    set(TARGET_FORMAT "elf64-littleaarch64")
    set(TARGET_ARCHITECTURE "aarch64")

    include(CMakeLists_linux.txt)

    add_executable(${PROJECT_NAME} ${SRCS_G} ${RES_SRC_FILE} ${RES_EMBED_FILES})
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/www/build/ DESTINATION ${VAR_DIR}/www)
endif ()

target_link_libraries(${PROJECT_NAME} ${MAIN_LIBS})