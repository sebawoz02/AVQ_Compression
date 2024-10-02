#pragma once

#include <block.hpp>

typedef struct Dictionary {
    size_t size;
    std::vector<Block*> entries;

    Dictionary();

    void insert(Block* new_block);
}Dictionary;
