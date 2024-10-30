#pragma once

#include <types/block.hpp>

struct Dict_entry {
  Dict_entry(Block* b, Dict_entry* n, Dict_entry* p)
    : block(b), usage_count(0), prev(p), next(n){};
  ~Dict_entry();

  Block* block;
  size_t usage_count;
  Dict_entry* prev;
  Dict_entry* next;
};

typedef struct Dictionary {
  Dictionary(): len(0), first_entry(nullptr), last_entry(nullptr){};
  ~Dictionary();

  void insert(Block* block);
  void remove(Block* block);
  [[nodiscard]] size_t size() const;

  [[maybe_unused]] [[nodiscard]] size_t get_count(size_t index) const;
  [[maybe_unused]] void inc_usage_count(Block* block);

  Block* operator[](size_t index) const;

private:
  size_t len;
  Dict_entry* first_entry;
  Dict_entry* last_entry;

  [[nodiscard]] Dict_entry* get_entry_at(size_t index) const;
} Dictionary;