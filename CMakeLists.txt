cmake_minimum_required(VERSION 3.5)

project(DotClock LANGUAGES CXX)

add_executable(clock main.cpp)
target_include_directories(clock PUBLIC .)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

