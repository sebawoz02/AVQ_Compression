#include <dictionary.hpp>
#include <stdexcept>

Dictionary::Dictionary()
{
  size = 0;
  entries = {};
}

void Dictionary::insert(Block* new_block)
{
  if(new_block == nullptr) {
    throw std::invalid_argument("Attempted to insert a null block");
  }
  entries.push_back(new_block);
  size++;
}

Dictionary::~Dictionary()
{
  for(Block* b: entries) {
    delete b;
  }
}
