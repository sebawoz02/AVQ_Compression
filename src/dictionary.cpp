#include <dictionary.hpp>

Dictionary::Dictionary()
{
    size = 0;
    entries = {nullptr};
}

void Dictionary::insert(Block* new_block)
{
    entries.push_back(new_block);
    size++;
}
