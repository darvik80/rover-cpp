cmake_minimum_required(VERSION 3.16)
project(service VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")

add_definitions(-DRASPBERRY_ARCH -DSUPPORT_ETHERNET)
if(UNIX AND NOT APPLE)
    add_definitions(-DSUPPORT_XBOXDRV)
    add_definitions(-DSUPPORT_BARCODE)
endif()

if (APPLE)
    set(OPENSSL_ROOT_DIR /usr/local/Cellar/openssl@1.1/1.1.1l)
endif ()

find_package(OpenSSL REQUIRED)
#find_package(ZeroMQ 4.0 REQUIRED)
#find_package(Azmq REQUIRED)
set(MAIN_SERVICE_LIBS logging event-manager mqtt zmq zmqpp)

### proto build {
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

# Generated sources
get_filename_component(PROTO "${PROJECT_SOURCE_DIR}/proto/helloworld.proto" ABSOLUTE)
get_filename_component(PROTO_PATH "${PROTO}" PATH)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO})
set(GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.cc")
set(GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}/helloworld.grpc.pb.h")

add_custom_command(
        OUTPUT "${GRPC_SRCS}" "${GRPC_HDRS}"
        COMMAND protoc
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}" -I "${PROTO_PATH}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${PROTO}"
        DEPENDS "${PROTO}")
### } proto build

include_directories(
        /usr/local/include
        /usr/include
        ${Protobuf_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../core-utils/logging/src
        ${PROJECT_SOURCE_DIR}/../core-utils/event-manager/src
        ${PROJECT_SOURCE_DIR}/../mqtt/src
        ${PROJECT_SOURCE_DIR}/../platform/src
        ${PROJECT_SOURCE_DIR}/../lib/platform-io/lib/core/src
)

file(GLOB SRCS_G
        src/*.cpp
        src/logging/*.cpp
        src/module/*.cpp
        src/properties/*.cpp
        src/properties/source/*.cpp
        src/module/raspberry/*.cpp
        src/mqtt/*.cpp
        src/net/*.cpp
        src/net/http/*.cpp
        src/net/http/rpc/*.cpp
        src/net/http/rpc/json/*.cpp
        src/serial/*.cpp
        src/serial/os/*.cc
        src/scheduler/*.cpp
        src/grpc/*.cpp
        src/joystick/*.cpp
        src/zeromq/*.cpp
        ${PROTO_SRCS}
        ${GRPC_SRCS}
        )

set(RESOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources)
set(WWW_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/www)

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

    add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRCS_G} ${SRCS_OS} ${RESOURCES_G} ${PROTO_SRCS} ${GRPC_SRCS})
else ()
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)
    set(PROPERTIES_DIR ${EXECUTABLE_OUTPUT_PATH}/../etc)
    set(PLUGINS_DIR ${EXECUTABLE_OUTPUT_PATH}/../lib)
    set(VAR_DIR ${EXECUTABLE_OUTPUT_PATH}/../var)

    if (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES arm)
        set(TARGET_FORMAT "elf32-littlearm")
        set(TARGET_ARCHITECTURE "arm")
    elseif (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES aarch64)
        set(TARGET_FORMAT "elf64-littleaarch64")
        set(TARGET_ARCHITECTURE "aarch64")
    endif ()

    include(CMakeLists_linux.txt)

    add_executable(${PROJECT_NAME} ${SRCS_G} ${RES_SRC_FILE} ${RES_EMBED_FILES} ${PROTO_SRCS} ${GRPC_SRCS})
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)

    #file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/www/build/ DESTINATION ${VAR_DIR}/www)
endif ()

add_subdirectory(testsuite)

target_link_libraries(${PROJECT_NAME} ${MAIN_LIBS} ${MAIN_SERVICE_LIBS} gRPC::grpc++ ${Protobuf_LIBRARIES})