#pragma once

#include <cstdint>

#define TGA_HEADER_SIZE 18

#pragma pack(push, 1)
struct TGA_header {
  uint8_t id_len;
  uint8_t cmp;
  uint8_t img_type;
  uint16_t cm_entry_idx;
  uint16_t cm_len;
  uint8_t cm_size;
  uint16_t x;
  uint16_t y;

  uint16_t width;
  uint16_t height;

  uint8_t pixel_bits;
  uint8_t descriptor;
};
#pragma pack(pop)