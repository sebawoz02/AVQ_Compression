#pragma once

#include <cstdint>
#include <utility>
#include <vector>

typedef struct Image {
  explicit Image(std::vector<std::vector<uint8_t>> _pixels, size_t _width,
                 size_t _height)
    : width(_width), height(_height), pixels(std::move(_pixels)),
      encoded(width, std::vector<bool>(height, false)){};

  size_t width;
  size_t height;
  std::vector<std::vector<uint8_t>> pixels;
  std::vector<std::vector<bool>> encoded;
} Image;
