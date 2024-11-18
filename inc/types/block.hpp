#pragma once
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

/**
 * @struct Block
 * @brief Represents 2D rectangle shaped block of values.
 */
typedef struct Block {
  size_t width;                                 ///< Block width x-axis
  size_t height;                                ///< Block height y-axis
  std::vector<std::vector<uint8_t>> pixels;     ///< Values that make up block

  Block(size_t w, size_t h, std::vector<std::vector<uint8_t>> p)
    : width(w), height(h), pixels(std::move(p)){};

  void mean_and_variance(double& mean, double& variance);
} Block;
