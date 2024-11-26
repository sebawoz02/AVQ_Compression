#pragma once

#include <types/dict_entry.hpp>

typedef struct Deletion_Handler {
  virtual void update(Dict_entry* entry) = 0;
  virtual Dict_entry* get_entry_to_delete() = 0;
  virtual ~Deletion_Handler() = default;
} Deletion_Handler;

struct FIFO_entry;

struct FIFO_Handler: public Deletion_Handler {
  void update(Dict_entry* entry) override;
  Dict_entry* get_entry_to_delete() override;
  ~FIFO_Handler() override;

  FIFO_entry* head;
  FIFO_entry* tail;
  int size = 0;
};