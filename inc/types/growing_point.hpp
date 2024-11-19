#pragma once

#include <cstddef>
#include <cstdint>

/**
 * @struct Growing_point
 * @brief Coordinates representing the corner of the block waiting to be encoded/decoded.
 */
typedef struct Growing_point {
  uint16_t x; ///< x coordinate
  uint16_t y; ///< y coordinate
  Growing_point(uint16_t _x, uint16_t _y): x(_x), y(_y){};
} Growing_point;
