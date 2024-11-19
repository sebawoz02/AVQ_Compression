#pragma once

#include <types/dictionary.hpp>
#include <types/growing_point.hpp>
#include <types/image.hpp>

#define GPP_SIZE_LIMIT 48

/**
 * @struct GP_pool_entry
 * @brief Represents Growing Points Pool entry
 */
typedef struct GP_pool_entry {
    explicit GP_pool_entry(Growing_point* _gp, GP_pool_entry* _prev): next(nullptr), prev(_prev), gp(_gp){};
    ~GP_pool_entry();

    GP_pool_entry* next;
    GP_pool_entry* prev;
    Growing_point* gp;
} GP_pool_entry;

/**
 * @struct GP_pool
 * @brief Growing Points Pool
 */
typedef struct GP_pool {
  ~GP_pool();
  GP_pool(): _size(0), head(nullptr), tail(nullptr){};

  GP_pool_entry* operator[](uint8_t index) const;
  [[nodiscard]] bool contains(size_t x, size_t y) const;

  void add(Growing_point* gp_new);
  void remove(GP_pool_entry* entry);
  void remove_obsolete(Image& image);

  [[__nodiscard__]] GP_pool_entry* last();
  [[__nodiscard__]] GP_pool_entry* first();
  [[__nodiscard__]] uint8_t size() const;

private:
  uint8_t _size;        ///< Growing Points Pool length/size
  GP_pool_entry* head;  ///< First entry
  GP_pool_entry* tail;  ///< Last entry
} GP_pool;
