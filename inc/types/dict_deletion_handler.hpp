#pragma once

#include <types/dict_entry.hpp>
#include <list>
#include <unordered_map>

typedef enum Deletion_Mode
{
    DELETION_MODE_NONE,
    DELETION_MODE_FIFO,
    DELETION_MODE_LRU,
} Deletion_Mode;

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

private:
  FIFO_entry* head;
  FIFO_entry* tail;
  int size = 0;
};

struct LRU_Handler: public Deletion_Handler {
    void update(Dict_entry* entry) override;
    Dict_entry* get_entry_to_delete() override;
private:
    std::list<Dict_entry*> items;
    std::unordered_map<Dict_entry*, std::list<Dict_entry*>::iterator> cache;
};