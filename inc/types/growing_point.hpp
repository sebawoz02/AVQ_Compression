#pragma once

#include <cstddef>
#include <cstdint>

/**
 * @struct Growing_point
 * @brief Coordinates representing the corner of the block waiting to be encoded/decoded.
 */
typedef struct Growing_point {
  size_t x; ///< x coordinate
  size_t y; ///< y coordinate
  Growing_point(size_t _x, size_t _y): x(_x), y(_y){};
} Growing_point;
