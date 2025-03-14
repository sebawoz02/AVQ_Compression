#include <stdexcept> // for std::out_of_range
#include <types/dictionary.hpp>

struct Dict_entry {
  Dict_entry(Block* b, Dict_entry* n, Dict_entry* p)
    : block(b), prev(p), next(n){};
  ~Dict_entry();

  Block* block;
  Dict_entry* prev;
  Dict_entry* next;
};

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
  delete deletion_handler;
}

void Dictionary::insert(Block* const block)
{
  if(len == 0) {
    first_entry = new Dict_entry(block, nullptr, nullptr);
    last_entry = first_entry;
    len++;
    if(deletion_mode == DELETION_MODE_FIFO || deletion_mode == DELETION_MODE_LRU) {
      deletion_handler->update(first_entry);
    }
    return;
  }
  if(len == 1) {
    len++;
    if(block->width * block->height >=
      first_entry->block->width * first_entry->block->height) {
      last_entry = new Dict_entry(block, nullptr, first_entry);
      first_entry->next = last_entry;
      if(deletion_mode == DELETION_MODE_FIFO || deletion_mode == DELETION_MODE_LRU) {
        deletion_handler->update(last_entry);
      }
      return;
    } else {
      first_entry = new Dict_entry(block, last_entry, nullptr);
      last_entry->prev = first_entry;
      if(deletion_mode == DELETION_MODE_FIFO || deletion_mode == DELETION_MODE_LRU) {
        deletion_handler->update(first_entry);
      }
      return;
    }
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
    if(deletion_mode == DELETION_MODE_FIFO || deletion_mode == DELETION_MODE_LRU) {
      deletion_handler->update(entry);
    }
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
  if(deletion_mode == DELETION_MODE_FIFO || deletion_mode == DELETION_MODE_LRU) {
    deletion_handler->update(entry);
  }
}

void Dictionary::remove(Block* const block)
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

Block* Dictionary::operator[](size_t const index) const
{
  return get_entry_at(index)->block;
}

Dict_entry* Dictionary::get_entry_at(size_t const index) const
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


void Dictionary::remove(size_t const index)
{
  Dict_entry* entry;
  len--;
  if(index == 0) {
    entry = first_entry;
    first_entry = entry->next;
    first_entry->prev = nullptr;
    delete first_entry;
    return;
  }

  entry = get_entry_at(index);

  entry->prev->next = entry->next;
  if(entry->next != nullptr) {
    entry->next->prev = entry->prev;
  }
  delete entry;
}

void Dictionary::delete_entry()
{
  len--;
  Dict_entry* entry_to_delete = deletion_handler->get_entry_to_delete();
  if(entry_to_delete->prev != nullptr) {
    entry_to_delete->prev->next = entry_to_delete->next;
  }
  if(entry_to_delete->next != nullptr) {
    entry_to_delete->next->prev = entry_to_delete->prev;
  } else {
    last_entry = entry_to_delete->prev;
  }
  delete entry_to_delete;
}

void Dictionary::set_deletion_mode(Deletion_Mode mode)
{
  deletion_mode = mode;
  if(mode == DELETION_MODE_FIFO) {
    deletion_handler = new FIFO_Handler();
  }
  else if(mode == DELETION_MODE_LRU) {
    deletion_handler = new LRU_Handler();
  }
}
