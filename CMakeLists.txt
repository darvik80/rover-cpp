cmake_minimum_required(VERSION 3.12)
project(rover VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    message("Apple build")
    find_library(IOKIT_LIBRARY IOKit)
    find_library(CORE_LIBRARY CoreFoundation)
else ()
    message("Unix build")
    set(CORE_LIBRARY dl atomic)
endif ()

find_package(Boost REQUIRED COMPONENTS system filesystem thread log log_setup)
find_package(Threads)

set(MAIN_LIBS ${Boost_LIBRARIES} ${IOKIT_LIBRARY} ${CORE_LIBRARY} logging)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fvisibility=hidden")

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-type-limits -Wno-psabi")
    include_directories(
            /usr/include/c++/8
            /usr/include
            /usr/local/include
    )
endif ()

include_directories(
        /usr/local/include
        core-utils/event-manager/src
        core-utils/logging/src
)

link_directories(
        /usr/local/lib
)

set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_INSTALL_NAME_DIR "")

enable_testing()

add_subdirectory(core-utils)
add_subdirectory(platform-io/lib/zeromq)
add_subdirectory(mqtt)
add_subdirectory(service)