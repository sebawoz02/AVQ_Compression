cmake_minimum_required(VERSION 3.22)
project(AVQ_compression)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(SOURCES src/encoder.cpp src/heuristics.cpp src/input_reader.cpp src/output_writer.cpp src/dictionary.cpp)
set(TEST_SOURCES test/src/test_heuristics.cpp)

add_executable(avq app/main.cpp ${SOURCES})
add_executable(test test/main.cpp ${SOURCES} ${TEST_SOURCES})

target_include_directories(avq PRIVATE inc)
target_include_directories(test PRIVATE inc test/inc)