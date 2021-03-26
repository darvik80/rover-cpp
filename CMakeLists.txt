cmake_minimum_required(VERSION 3.12)
project(rover VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    message("Apple build")
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_LIBRARY CoreFoundation)
else ()
    message("Unix build")
endif ()

find_package(Boost REQUIRED COMPONENTS system filesystem thread log log_setup)
find_package(Threads)

set(MAIN_LIBS ${Boost_LIBRARIES} ${IOKIT_LIBRARY} ${CORE_LIBRARY})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-type-limits -Wno-psabi")
    include_directories(
            /usr/include/c++/8
            /usr/include
            /usr/local/include
    )
endif ()

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_NAME_DIR "")

enable_testing()

#add_subdirectory(lib/arduino)
#add_subdirectory(lib/json)
#add_subdirectory(lib/serial)
add_subdirectory(lib/mqtt)
add_subdirectory(platform)
add_subdirectory(plugins/system)
#add_subdirectory(service/www)
add_subdirectory(service)

add_subdirectory(incubator)