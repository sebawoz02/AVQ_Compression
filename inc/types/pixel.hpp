#pragma once

#include <cstdint>

/**
 * @struct Pixel
 * @brief RGB pixel representation
 */
struct Pixel {
  Pixel(uint8_t r, uint8_t g, uint8_t b): blue(b), green(g), red(r){};

  uint8_t blue;     ///> Blue color value 0-255
  uint8_t green;    ///> Green color value 0-255
  uint8_t red;      ///> Red color value 0-255
};