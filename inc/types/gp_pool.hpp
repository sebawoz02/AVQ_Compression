#pragma once

#include <cstddef>
#include <types/growing_point.hpp>
#include <types/image.hpp>

#define GPP_SIZE_LIMIT 48

/**
 * @struct GP_pool_entry
 * @brief Represents Growing Points Pool entry
 */
typedef struct GP_pool_entry {
    explicit GP_pool_entry(Growing_point* _gp): next(nullptr), gp(_gp){};
    ~GP_pool_entry();

    GP_pool_entry* next;
    Growing_point* gp;
} GP_pool_entry;


/**
 * @struct GP_pool
 * @brief Growing Points Pool
 */
typedef struct GP_pool {
  ~GP_pool();
  GP_pool(): head(nullptr), tail(nullptr), _size(0){};

  Growing_point* operator[](size_t index) const;
  [[nodiscard]] bool contains(size_t x, size_t y) const;

  void add(Growing_point* gp_new);
  void remove(Growing_point* gp_old);
  void remove_obsolete(Image& image);

  [[__nodiscard__]] Growing_point* last();
  [[__nodiscard__]] GP_pool_entry* first();
  [[__nodiscard__]] size_t size() const;

private:
  GP_pool_entry* head;  ///< First entry
  GP_pool_entry* tail;  ///< Last entry
  size_t _size;         ///< Growing Points Pool length/size
} GP_pool;
