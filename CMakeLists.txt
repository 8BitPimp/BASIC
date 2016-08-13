cmake_minimum_required(VERSION 3.6)
project(basic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp builder.cpp builder.h opcodes.h)
add_executable(basic ${SOURCE_FILES})