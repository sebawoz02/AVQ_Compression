#pragma once

#include <cstdint>

struct Pixel {
  Pixel(uint8_t r, uint8_t g, uint8_t b): blue(b), green(g), red(r){};

  uint8_t blue;
  uint8_t green;
  uint8_t red;
};