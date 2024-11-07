#pragma once

#include <cstdint>
#include <fstream>
#include <types/header_tga.hpp>
#include <types/pixel.hpp>
#include <vector>

typedef enum Additional_Compression_Mode {
  NO_ADDITIONAL_COMPRESSION = 0,
  AC_ENCODING = 1,
  AC_DECODING = 2,
} Additional_Compression_Mode;

typedef struct IO_Handler {
  IO_Handler(char* in_filename, char* out_filename,
             Additional_Compression_Mode _ac);
  ~IO_Handler();

  void get_header(TGA_header* header);
  void get_image(size_t width, size_t height,
                 std::vector<std::vector<Pixel>>* image);
  void write_header(const TGA_header& header);
  void write_image(const std::vector<std::vector<Pixel>>& image, size_t width,
                   size_t height);

  uint16_t read_bits(size_t x);
  void write(bool bit);
  void flush_buffer();

  [[nodiscard]] uint64_t get_bytes_read() const;
  void set_print_summary(bool ps);

private:
  uint8_t in_buffer;
  uint8_t out_buffer;

  size_t in_bit_count;
  size_t out_bit_count;

  uint64_t bytes_read;
  uint64_t bytes_wrote;

  std::ifstream in_stream;
  std::ofstream out_stream;

  bool print_summary;
  Additional_Compression_Mode ac_mode;
} IO_Handler;