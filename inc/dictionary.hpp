#pragma once

#include <block.hpp>

typedef struct Dictionary {
    size_t size;
    std::vector<Block*> entries;
}Dictionary;
