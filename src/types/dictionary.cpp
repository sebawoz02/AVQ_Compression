#include <stdexcept> // for std::out_of_range
#include <types/dictionary.hpp>

Dict_entry::~Dict_entry()
{
  delete block;
}

Dictionary::~Dictionary()
{
  for(size_t i = 0; i < len; ++i) {
    Dict_entry* entry = last_entry;
    last_entry = entry->prev;
    delete entry;
  }
}

void Dictionary::insert(Block* block)
{
  if(len == 0) {
    first_entry = new Dict_entry(block, nullptr, nullptr);
    last_entry = first_entry;
    len++;
    return;
  }
  if(len == 1) {
    len++;
    if(block->width * block->height >=
       first_entry->block->width * first_entry->block->height) {
      last_entry = new Dict_entry(block, nullptr, first_entry);
      first_entry->next = last_entry;
    } else {
      first_entry = new Dict_entry(block, last_entry, nullptr);
      last_entry->prev = first_entry;
    }
    return;
  }
  len++;

  Dict_entry* prev = last_entry;
  size_t block_size = block->width * block->height;
  while(prev != nullptr &&
        prev->block->width * prev->block->height > block_size) {
    prev = prev->prev;
  }
  if(prev == nullptr) {
    auto* entry = new Dict_entry(block, first_entry, nullptr);
    first_entry->prev = entry;
    first_entry = entry;
    return;
  }

  auto* entry = new Dict_entry(block, prev->next, prev);
  if(prev->next != nullptr) {
    prev->next->prev = entry;
  }
  prev->next = entry;

  if(prev == last_entry) {
    last_entry = entry;
  }
}

void Dictionary::remove(Block* block)
{
  if(len == 0) {
    return;
  }
  len--;
  Dict_entry* entry_to_remove = last_entry;

  while(entry_to_remove != nullptr && block != entry_to_remove->block) {
    entry_to_remove = entry_to_remove->prev;
  }

  if(entry_to_remove != nullptr) {
    if(entry_to_remove == first_entry) {
      first_entry = entry_to_remove->next;
      if(entry_to_remove->next != nullptr) {
        entry_to_remove->next->prev = nullptr;
      }
    } else if(entry_to_remove == last_entry) {
      last_entry = entry_to_remove->prev;
      if(entry_to_remove->prev != nullptr) {
        entry_to_remove->prev->next = nullptr;
      }
    } else {
      entry_to_remove->next->prev = entry_to_remove->prev;
      entry_to_remove->prev->next = entry_to_remove->next;
    }
  }
  delete entry_to_remove;
}

size_t Dictionary::size() const
{
  return len;
}

Block* Dictionary::operator[](size_t index) const
{
  return get_entry_at(index)->block;
}

[[maybe_unused]] size_t Dictionary::get_count(size_t index) const
{
  return get_entry_at(index)->usage_count;
}

Dict_entry* Dictionary::get_entry_at(size_t index) const
{
  if(index >= len) {
    throw std::out_of_range("Dictionary index out of range");
  }

  if(index > len / 2) {
    Dict_entry* entry = last_entry;
    for(size_t i = len - 1; i > index; i--) {
      entry = entry->prev;
    }
    return entry;
  } else {
    Dict_entry* entry = first_entry;
    for(size_t i = 0; i < index; i++) {
      entry = entry->next;
    }
    return entry;
  }
}

[[maybe_unused]] void Dictionary::inc_usage_count(Block* block)
{
  Dict_entry* ent = first_entry;
  while(ent != nullptr && ent->block != block) {
    ent = ent->next;
  }
  if(ent != nullptr) {
    ent->usage_count++;
  }
}
