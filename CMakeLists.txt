cmake_minimum_required (VERSION 3.12)

project ("JiuPlayer")

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDEBUG)
endif()

set(CMAKE_PREFIX_PATH ${BoostPath})
find_package(Boost REQUIRED COMPONENTS date_time regex)
if(Boost_FOUND)
    message(${Boost_VERSION})
    include_directories(${Boost_INCLUDE_DIRS})
endif()

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory ("Common")
add_subdirectory ("Player")