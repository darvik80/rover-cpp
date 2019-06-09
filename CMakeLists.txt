cmake_minimum_required(VERSION 3.7)
project(rover VERSION 1.0)

file(GLOB SRCS_G
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/handlers/*.cpp
        ${PROJECT_SOURCE_DIR}/src/handlers/rpc/*.cpp
        ${PROJECT_SOURCE_DIR}/src/codec/*.cpp
        ${PROJECT_SOURCE_DIR}/src/codec/json/*.cpp
        )

message(INCLUDE, ${CMAKE_BINARY_DIR})
message(INCLUDE, ${CMAKE_SOURCE_DIR})
message(INCLUDE, ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(/usr/local/macchina/include ${PROJECT_SOURCE_DIR}/src)
link_directories(/usr/local/macchina/lib)

#find_package(Poco REQUIRED COMPONENTS Foundation Util JSON)

add_executable(rover ${SRCS_G})

target_link_libraries(rover PUBLIC
    ${CMAKE_THREAD_LIBS_INIT}
        -lPocoUtil
        -lPocoFoundation
        -lPocoNet
        -lPocoJSON
)
