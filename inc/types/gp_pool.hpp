#pragma once

#include <cstddef>
#include <types/growing_point.hpp>
#include <types/image.hpp>

typedef struct GP_pool_entry {
  explicit GP_pool_entry(Growing_point* _gp): next(nullptr), gp(_gp){};
  ~GP_pool_entry();

  GP_pool_entry* next;
  Growing_point* gp;
} GP_pool_entry;

typedef struct GP_pool {
  ~GP_pool();
  GP_pool(): head(nullptr), _size(0){};

  Growing_point* operator[](size_t index) const;
  [[nodiscard]] bool contains(size_t x, size_t y) const;

  void add(Growing_point* gp_new);
  void remove(Growing_point* gp_old);
  void remove_obsolete(Image& image);

  [[__nodiscard__]] Growing_point* last();
  [[__nodiscard__]] size_t size() const;

private:
  GP_pool_entry* head;
  size_t _size;
} GP_pool;
