#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

typedef struct Block {
    size_t width;
    size_t height;
    std::vector<std::vector<uint8_t>> pixels;
} Block;