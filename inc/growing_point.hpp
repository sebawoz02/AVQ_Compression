#pragma once

#include <block.hpp>

typedef struct Growing_point {
    size_t x;
    size_t y;
    Block* block;

    Growing_point(size_t _x, size_t _y, Block* _b) : x(_x), y(_y), block(_b){};
    Growing_point(size_t _x, size_t _y, size_t width, size_t height, ColorMap* src);
    ~Growing_point();
} Growing_point;

