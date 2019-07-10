cmake_minimum_required(VERSION 3.0)
project(platform VERSION 1.0)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(NO_OUTPUT_DIRS)
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

file(GLOB SRCS_G src/json/*.cpp src/plugin/*.cpp src/rpc/*.cpp)

include_directories(
        /usr/local/include
        ${PROJECT_SOURCE_DIR}/src
)

link_directories(
        /usr/local/lib
)

message(test ${LIBRARY_OUTPUT_DIRECTORY})

add_library(platform STATIC ${SRCS_G})

add_subdirectory(testsuite)

target_link_libraries(platform ${CONAN_LIBS})
