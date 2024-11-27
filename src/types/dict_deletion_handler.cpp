#include <iostream>
#include <types/dict_deletion_handler.hpp>

struct FIFO_entry {
  FIFO_entry* next;
  Dict_entry* value;

  explicit FIFO_entry(Dict_entry* _value): next(nullptr), value(_value){};
};

void FIFO_Handler::update(Dict_entry* entry)
{
  size++;
  if(tail == nullptr) {
    head = new FIFO_entry(entry);
    tail = head;
    return;
  }
  tail->next = new FIFO_entry(entry);
  tail = tail->next;
}

Dict_entry* FIFO_Handler::get_entry_to_delete()
{
  Dict_entry* to_delete = head->value;
  FIFO_entry* prev_head = head;
  head = head->next;
  delete prev_head;
  return to_delete;
}

FIFO_Handler::~FIFO_Handler()
{
  FIFO_entry* ptr = head;
  while(ptr != tail) {
    FIFO_entry* to_delete = ptr;
    ptr = ptr->next;
    delete to_delete;
  }
  delete ptr;
}

void LRU_Handler::update(Dict_entry* entry) {
    auto it = cache.find(entry);
    if(it != cache.end())
    {
        items.splice(items.begin(), items, it->second);
        return;
    }

    items.push_front(entry);
    cache[entry] = items.begin();
}

Dict_entry* LRU_Handler::get_entry_to_delete() {
    Dict_entry* lru = items.back();
    cache.erase(lru);
    items.pop_back();
    return lru;
}
