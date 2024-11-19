#pragma once

#include <types/block.hpp>

#define DICT_SIZE_LIMIT 512

/**
 * @struct Dict_entry
 * @brief Holds all information about one dictionary entry
 */
struct Dict_entry;

/**
 * @struct Dictionary
 * @brief Dictionary structure with all blocks used to match with Growing Points.
 */
typedef struct Dictionary {
  Dictionary(): len(0), first_entry(nullptr), last_entry(nullptr){};
  ~Dictionary();

  void insert(Block* block);
  void remove(Block* block);
  void remove(size_t index);
  [[nodiscard]] size_t size() const;

  [[maybe_unused]] [[nodiscard]] size_t get_count(size_t index) const;
  [[maybe_unused]] void inc_usage_count(Block* block);

  Block* operator[](size_t index) const;

private:
  uint16_t len;             ///< Dictionaries length
  Dict_entry* first_entry;  ///< First entry - head
  Dict_entry* last_entry;   ///< Last entry - tail

  [[nodiscard]] Dict_entry* get_entry_at(size_t index) const;
} Dictionary;