#pragma once

#include <types/block.hpp>
#include <types/dict_entry.hpp>
#include <types/dict_deletion_handler.hpp>

#define DICT_SIZE_LIMIT 512

/**
 * @struct Dictionary
 * @brief Dictionary structure with all blocks used to match with Growing Points.
 */
typedef struct Dictionary {
  Dictionary(): deletion_mode(NONE), deletion_handler(nullptr), len(0), first_entry(nullptr), last_entry(nullptr){};
  ~Dictionary();

  void insert(Block* block);
  void remove(Block* block);
  void remove(size_t index);
  void set_deletion_mode(Deletion_Mode mode);
  void delete_entry();
  [[nodiscard]] size_t size() const;
  [[nodiscard]] Dict_entry* get_entry_at(size_t index) const;

  Block* operator[](size_t index) const;

  Deletion_Mode deletion_mode;
  Deletion_Handler* deletion_handler;
private:
  uint16_t len;             ///< Dictionaries length
  Dict_entry* first_entry;  ///< First entry - head
  Dict_entry* last_entry;   ///< Last entry - tail
} Dictionary;