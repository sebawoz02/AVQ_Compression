#pragma once
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

typedef struct Block {
    size_t width;
    size_t height;
    std::vector<std::vector<uint8_t>>& pixels;

    Block(size_t w, size_t h, std::vector<std::vector<uint8_t>>& p) : width(w), height(h), pixels(p){};
} Block;
