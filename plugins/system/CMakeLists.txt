cmake_minimum_required(VERSION 3.0)
project(system VERSION 1.0)

set(CMAKE_MACOSX_RPATH 1)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(NO_OUTPUT_DIRS)
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SRCS_G src/*.cpp)

include_directories(
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../../platform/src
)

link_directories(
        /usr/local/lib
)

message(${PROJECT_SOURCE_DIR}/../platform/src)

add_library(system SHARED ${SRCS_G} src/dllEntry.cpp)
add_subdirectory(testsuite)

target_link_libraries(system platform ${CONAN_LIBS})

SET(CMAKE_LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../lib)
