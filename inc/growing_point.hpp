#pragma once

#include <cstdint>

typedef struct Growing_point {
  size_t x;
  size_t y;
  Growing_point(size_t _x, size_t _y): x(_x), y(_y){};
} Growing_point;
