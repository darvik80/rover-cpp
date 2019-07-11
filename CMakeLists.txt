cmake_minimum_required(VERSION 3.0)
project(rover VERSION 1.0)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(NO_OUTPUT_DIRS)
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_NAME_DIR "")

enable_testing()

add_subdirectory(platform)
add_subdirectory(service)
add_subdirectory(plugins/system)

add_subdirectory(incubator)