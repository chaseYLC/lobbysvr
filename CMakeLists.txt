cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED YES)
set(CXX_EXTENSIONS NO)
add_compile_options(-fms-extensions)
add_compile_options(-fmacro-backtrace-limit=0)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdeclspec -fms-extensions -fmacro-backtrace-limit=0 -std=c++17" FORCE)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(lobbysvr)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

add_subdirectory(net)
add_subdirectory(lobbysvr)