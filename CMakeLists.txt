cmake_minimum_required(VERSION 3.1)
project(catcoder)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "-std=c99")

set(SOURCE_FILES bowling.c)
add_executable(catcoder ${SOURCE_FILES} bowling.c)