#include <types/gp_pool.hpp>
#include <stdexcept>


typedef struct GP_pool_entry {
  explicit GP_pool_entry(Growing_point* _gp, GP_pool_entry* _prev): next(nullptr), prev(_prev), gp(_gp){};
  ~GP_pool_entry();

  GP_pool_entry* next;
  GP_pool_entry* prev;
  Growing_point* gp;
} GP_pool_entry;

GP_pool::~GP_pool()
{
  GP_pool_entry* ptr = head;
  while(ptr != nullptr) {
    GP_pool_entry* to_delete = ptr;
    ptr = ptr->next;
    delete to_delete;
  }
}

Growing_point* GP_pool::operator[](uint8_t index) const
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
  return entry->gp;

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

void GP_pool::remove(Growing_point* gp_old)
{
  if(head == nullptr)
  {
    return;
  }

  if(head->gp == gp_old) {
    if(head == tail)
    {
         tail = nullptr;
    }
    GP_pool_entry* to_delete = head;
    head = head->next;
    delete to_delete;
    _size--;
    return;
  }

  GP_pool_entry* ptr = head->next;
  while(ptr != nullptr && ptr->gp != gp_old) {
    ptr = ptr->next;
  }

  if(ptr == nullptr)
  {
    return;
  }

  if(ptr == tail)
  {
    tail = ptr->prev;
  }
  delete ptr;
  _size--;
}

Growing_point* GP_pool::last()
{
  if(tail == nullptr)
  {
      return nullptr;
  }
  return tail->gp;
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
