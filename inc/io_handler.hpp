#pragma once

#include <cstdint>
#include <fstream>
#include <types/header_tga.hpp>
#include <types/pixel.hpp>
#include <vector>

typedef enum File_Format_E
{
    FORMAT_UNKNOWN,
    FORMAT_TGA,
    FORMAT_PGM,
} File_Format_E;

/**
 * @struct IO_Handler
 * @brief Structure responsible for all input and output file handling
 */
typedef struct IO_Handler {
  IO_Handler(char* in_filename, char* out_filename);
  ~IO_Handler();

  void get_header_tga(TGA_header* header);
  void get_image_rgb(size_t width, size_t height,
                 std::vector<std::vector<Pixel>>* image);
  void write_header_tga(const TGA_header& header);
  void write_image_rgb(const std::vector<std::vector<Pixel>>& image, size_t width,
                   size_t height);

  uint16_t read_bits(size_t x);
  void write(bool bit);
  void flush_buffer();

  [[nodiscard]] uint64_t get_bytes_read() const;
  void set_print_summary(bool ps);

  File_Format_E input_format;   ///> Input file format
  File_Format_E output_format;  ///> Input file format
private:
  uint8_t in_buffer;            ///> Input buffer - used to read single bits
  uint8_t out_buffer;           ///> Output buffer - used to write single bits

  size_t in_bit_count;          ///> Input buffer index
  size_t out_bit_count;         ///> Output buffer index

  uint64_t bytes_read;          ///> Bytes read
  uint64_t bytes_wrote;         ///> Bytes wrote

  std::ifstream in_stream;      ///> Input stream
  std::ofstream out_stream;     ///> Output stream

  bool print_summary;           ///> If true summary is printed at the destruction
} IO_Handler;