#pragma once

#include <types/dictionary.hpp>
#include <types/growing_point.hpp>
#include <types/image.hpp>

#define GPP_SIZE_LIMIT 48

/**
 * @struct GP_pool_entry
 * @brief Represents Growing Points Pool entry
 */
struct GP_pool_entry;

/**
 * @struct GP_pool
 * @brief Growing Points Pool
 */
typedef struct GP_pool {
  ~GP_pool();
  GP_pool(): head(nullptr), tail(nullptr), _size(0){};

  Growing_point* operator[](uint8_t index) const;
  [[nodiscard]] bool contains(size_t x, size_t y) const;

  void add(Growing_point* gp_new);
  void remove(Growing_point* gp_old);
  void remove_obsolete(Image& image);

  [[__nodiscard__]] Growing_point* last();
  [[__nodiscard__]] uint8_t size() const;

private:
  GP_pool_entry* head;  ///< First entry
  GP_pool_entry* tail;  ///< Last entry
  uint8_t _size;        ///< Growing Points Pool length/size
} GP_pool;
