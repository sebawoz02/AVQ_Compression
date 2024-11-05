cmake_minimum_required(VERSION 3.22)
project(AVQ_compression)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(SOURCES src/encoder.cpp src/heuristics.cpp src/io_handler.cpp src/decoder.cpp src/arg_parser.cpp
        src/types/dictionary.cpp src/types/gp_pool.cpp src/types/image.cpp src/types/block.cpp)
set(TEST_SOURCES test/src/test_heuristics.cpp test/src/test_dictionary.cpp test/src/test_gp_pool.cpp)

add_executable(avq app/avq/main.cpp ${SOURCES})
add_executable(analyzer app/analyzer/main.cpp src/io_handler.cpp)
add_executable(test test/main.cpp ${SOURCES} ${TEST_SOURCES})

target_include_directories(avq PRIVATE inc)
target_include_directories(analyzer PRIVATE inc)
target_include_directories(test PRIVATE inc test/inc)