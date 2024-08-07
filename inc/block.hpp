#pragma once
#include <cstddef>
#include <types.hpp>
#include <utility>
#include <vector>

typedef struct Block {
    size_t width;
    size_t height;
    ColorMap* pixels;

    Block(size_t w, size_t h, ColorMap* p) : width(w), height(h), pixels(p){};
    ~Block(){delete pixels;};
} Block;
