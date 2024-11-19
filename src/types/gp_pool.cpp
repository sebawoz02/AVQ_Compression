#include <types/gp_pool.hpp>
#include <stdexcept>


GP_pool::~GP_pool()
{
  GP_pool_entry* ptr = head;
  while(ptr != nullptr) {
    GP_pool_entry* to_delete = ptr;
    ptr = ptr->next;
    delete to_delete;
  }
}

GP_pool_entry* GP_pool::operator[](uint8_t index) const
{
  if(index >= _size)
  {
    throw std::out_of_range("GPP index out of range");
  }

  GP_pool_entry* entry;
  if(index > _size / 2) {
    entry = tail;
    for(uint8_t i = _size - 1; i > index; i--) {
      entry = entry->prev;
    }
  } else {
    entry = head;
    for(uint8_t i = 0; i < index; i++) {
      entry = entry->next;
    }
  }
  return entry;
}

bool GP_pool::contains(size_t x, size_t y) const
{
  GP_pool_entry* ptr = head;
  while(ptr != nullptr) {
    if(ptr->gp->x == x && ptr->gp->y == y) {
      return true;
    }
    ptr = ptr->next;
  }
  return false;
}

void GP_pool::add(Growing_point* new_gp)
{
  if(head == nullptr) {
    head = new GP_pool_entry(new_gp, nullptr);
    tail = head;
    _size++;
    return;
  }

  auto* gpp_new = new GP_pool_entry(new_gp, tail);
  tail->next = gpp_new;
  tail = gpp_new;
  _size++;
}

GP_pool_entry* GP_pool::last()
{
  return tail;
}

GP_pool_entry::~GP_pool_entry()
{
  if(next != nullptr)
  {
      next->prev = prev;
  }
  if(prev != nullptr)
  {
      prev->next = next;
  }
  delete gp;
}

void GP_pool::remove_obsolete(Image& image)
{
  GP_pool_entry* cur = head;
  while(cur != nullptr) {
    if(image.encoded[cur->gp->x][cur->gp->y]) {
      GP_pool_entry* to_rm = cur;
      if(cur == head) {
        head = cur->next;
        cur = head;
      } else if(cur == tail) {
        tail = cur->prev;
        cur = nullptr;
      } else
      {
        cur = cur->next;
      }
      delete to_rm;
      _size--;
    } else {
      cur = cur->next;
    }
  }
}

uint8_t GP_pool::size() const
{
  return _size;
}

void GP_pool::remove(GP_pool_entry* entry) {
    if(entry == head)
    {
        head = entry->next;
    }else if(entry == tail)
    {
        tail = entry->prev;
    }
    delete entry;
    _size--;
}

GP_pool_entry *GP_pool::first() {
   return head;
}
