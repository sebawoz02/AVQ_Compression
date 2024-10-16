#pragma once

#include <types/block.hpp>

struct Dict_list_entry {
  Dict_list_entry(Block* b, Dict_list_entry* n, Dict_list_entry* p)
    : block(b), usage_count(0), prev(p), next(n){};
  ~Dict_list_entry();

  Block* block;
  size_t usage_count;
  Dict_list_entry* prev;
  Dict_list_entry* next;
};

typedef struct Dict_list {
  Dict_list(): len(0), first_entry(nullptr), last_entry(nullptr){};
  ~Dict_list();

  void insert(Block* block);
  void remove(Block* block);
  [[nodiscard]] size_t size() const;
  [[nodiscard]] size_t get_count(size_t index) const;
  void inc_usage_count(Block* block);

  Block* operator[](size_t index) const;

private:
  size_t len;
  Dict_list_entry* first_entry;
  Dict_list_entry* last_entry;

  [[nodiscard]] Dict_list_entry* get_entry_at(size_t index) const;
} Dict_list;