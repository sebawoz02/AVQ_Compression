#pragma once

#include <block.hpp>
#include <cstdint>

typedef struct Growing_point {
    size_t x;
    size_t y;
    Block* block;

    Growing_point(size_t _x, size_t _y, Block* _b) : x(_x), y(_y), block(_b){};
    Growing_point(size_t _x, size_t _y, size_t width, size_t height,  std::vector<std::vector<uint8_t>>& src);
} Growing_point;

