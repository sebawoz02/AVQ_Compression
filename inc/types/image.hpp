#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

/**
 * @struct Image
 * @brief One-color image. 2D bitmap.
 */
typedef struct Image {
  explicit Image(std::vector<std::vector<uint8_t>> _pixels, size_t _width,
                 size_t _height)
    : width(_width), height(_height), pixels(std::move(_pixels)),
      encoded(width, std::vector<bool>(height, false)){};

  size_t width;                                 ///> Image width
  size_t height;                                ///> Image height
  std::vector<std::vector<uint8_t>> pixels;     ///> 2D vector holding color values 0-255
  std::vector<std::vector<bool>> encoded;       ///> Bitfield telling which part was encoded/decoded

  [[maybe_unused]] bool encoded_at(size_t x, size_t y, size_t w, size_t h);
} Image;
