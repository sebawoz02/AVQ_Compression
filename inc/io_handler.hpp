#pragma once

#include <cstdint>
#include <fstream>
#include <types/pixel.hpp>
#include <vector>
#include <types/header_tga.hpp>

typedef struct IO_Handler {
  IO_Handler(char* in_filename, char* out_filename);
  ~IO_Handler();

  void get_header(TGA_header* header);
  void get_image(size_t width, size_t height,
                 std::vector<std::vector<Pixel>>* image);
  void write_header(const TGA_header& header);
  void write_image(const std::vector<std::vector<Pixel>>& image);


  uint16_t read_bits(size_t x);
  void write(bool bit);
  void flush_buffer();

private:
  std::ifstream in_stream;
  uint8_t in_buffer;
  size_t in_bit_count;

  std::ofstream out_stream;
  size_t out_bit_count;
  uint8_t out_buffer;
} IO_Handler;