cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED YES)
set(CXX_EXTENSIONS NO)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -fmacro-backtrace-limit=0 -std=c++14")

project(lobbysvr)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

add_subdirectory(lobbysvr)