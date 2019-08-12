cmake_minimum_required(VERSION 3.2)
project(rover VERSION 1.0)

set (MAIN_LIBS "empty")
if (APPLE)
    message("Apple build")
    if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        conan_basic_setup(NO_OUTPUT_DIRS)
        set (MAIN_LIBS ${CONAN_LIBS})
    else()
        message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
    endif()
else()
    message("Unix build")
    find_package(Boost COMPONENTS date_time filesystem system log thread)
    if(Boost_FOUND)
        set(MAIN_LIBS ${Boost_LIBRARIES} )
        set(PLATFORM_SPECIFIC_LIBS pthread)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LOG_DYN_LINK")

    else()
        message("Please install boot  >= 1.67")
    endif()
endif()

message(": " ${MAIN_LIBS})

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_NAME_DIR "")

enable_testing()

add_subdirectory(platform)
add_subdirectory(service)
add_subdirectory(plugins/system)

add_subdirectory(incubator)