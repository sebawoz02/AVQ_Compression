#pragma once

#include <cstdint>
#include <fstream>
#include <types/pixel.hpp>
#include <string>
#include <vector>

namespace input_reader {
  typedef struct InReader {
  public:
    explicit InReader(char* input_name);
    std::vector<std::vector<Pixel>>& get_image();

  private:
    std::string mode;
    std::vector<std::vector<Pixel>> image;

  } InReader;
} // namespace input_reader