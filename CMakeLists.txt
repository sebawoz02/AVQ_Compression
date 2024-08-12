cmake_minimum_required(VERSION 3.22)
project(AVQ_compression)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(SOURCES src/pixel.cpp src/encoder.cpp src/heuristics.cpp src/input_reader.cpp src/growing_point.cpp src/output_writer.cpp)

add_executable(avq app/main.cpp ${SOURCES})

target_include_directories(avq PRIVATE inc)