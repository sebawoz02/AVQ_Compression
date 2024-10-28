#pragma once

#include <cstdint>

struct Pixel {
  Pixel(uint8_t r, uint8_t g, uint8_t b): red(r), green(g), blue(b){};

  uint8_t red;
  uint8_t green;
  uint8_t blue;
};