cmake_minimum_required(VERSION 3.14)
project(rover VERSION 1.0 LANGUAGES CXX)

file(GLOB SRCS_G ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/handlers/*.cpp)

message(INCLUDE, ${CMAKE_BINARY_DIR})
message(INCLUDE, ${CMAKE_SOURCE_DIR})
message(INCLUDE, ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(/usr/local/include)

find_package(Poco REQUIRED COMPONENTS Foundation Util Net JSON)

add_executable(rover ${SRCS_G})

target_link_libraries(rover PUBLIC
    Poco::Foundation
    Poco::Util
    Poco::Net
    Poco::JSON
    ${CMAKE_THREAD_LIBS_INIT}
)
