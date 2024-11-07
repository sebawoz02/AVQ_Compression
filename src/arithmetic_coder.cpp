#include <arithmetic_coder.hpp>
#include <cmath>
#include <iostream>

#define NO_SYMBOLS 256

constexpr uint64_t precision = 32;
constexpr uint64_t whole = (uint64_t)1 << precision;
constexpr uint64_t half = whole / 2;
constexpr uint64_t quart = whole / 4;

[[nodiscard]] static uint16_t
insert_to_buffer_helper(uint8_t* bit_buffer, size_t* bit_buff_idx, uint8_t bit)
{
  if(bit)
    *bit_buffer |= (1 << (*bit_buff_idx));
  else
    *bit_buffer &= ~(1 << (*bit_buff_idx));
  if(*bit_buff_idx == 0) {
    *bit_buff_idx = 7;

    uint16_t write = *bit_buffer;
    *bit_buffer = 0;
    return write;
  } else {
    (*bit_buff_idx)--;
    return NO_RETURN;
  }
}

static void insert_to_buffer(uint8_t* bit_buffer, size_t* bit_buff_idx,
                             uint8_t bit, std::vector<uint8_t>* bytes)
{
  uint16_t byte = insert_to_buffer_helper(bit_buffer, bit_buff_idx, bit);
  if(byte == NO_RETURN) {
    return;
  }
  bytes->push_back(byte);
}

static uint8_t get_bit(uint8_t* byte, size_t* bit_index,
                       std::ifstream* in_stream, bool* eof)
{
  uint8_t bit = (*byte >> *bit_index) & 1;
  if(*bit_index == 0) {
    char tmp;
    in_stream->get(tmp);
    if(tmp == EOF) {
      *eof = true;
    }
    *byte = tmp;
    *bit_index = 7;
  } else
    (*bit_index)--;
  return bit;
}

Arithmetic_Coder::Arithmetic_Coder()
{
  bit_buffer = 0; // Bit buffer
  bit_buff_idx = 7; // Index in buffer
  l = 0; // Lower limit
  u = whole; // Upper limit
  scale = 0; // Scale
  occur = new uint64_t[NO_SYMBOLS];
  for(size_t i = 0; i < NO_SYMBOLS; i++) {
    occur[i] = 1;
  }
  input_size = NO_SYMBOLS;
}

std::vector<uint8_t>* Arithmetic_Coder::encode(uint8_t sym)
{
  auto* bytes = new std::vector<uint8_t>;

  uint64_t cumulative_count = 0;
  for(int16_t i = 0; i < sym; i++)
    cumulative_count += occur[i];

  // Update low and up
  uint64_t dist = u - l;
  u = l + static_cast<uint64_t>(
            roundl(static_cast<double>(dist) *
                   (static_cast<double>(cumulative_count + occur[sym]) /
                    static_cast<double>(input_size))));
  l = l + static_cast<uint64_t>(roundl(static_cast<double>(dist) *
                                       (static_cast<double>(cumulative_count) /
                                        static_cast<double>(input_size))));

  while(true) {
    if(u < half) {
      insert_to_buffer(&bit_buffer, &bit_buff_idx, 0, bytes);
      while(scale > 0) {
        insert_to_buffer(&bit_buffer, &bit_buff_idx, 1, bytes);
        scale--;
      }
    } else if(l > half) {
      l -= half;
      u -= half;
      insert_to_buffer(&bit_buffer, &bit_buff_idx, 1, bytes);
      while(scale > 0) {
        insert_to_buffer(&bit_buffer, &bit_buff_idx, 0, bytes);
        scale--;
      }
    } else if(l > quart && u < 3 * quart) {
      l -= quart;
      u -= quart;
      scale++;
    } else
      break;
    u <<= 1;
    l <<= 1;
  }
  occur[sym]++;
  input_size++;

  return bytes;
}

uint16_t Arithmetic_Coder::flush_buffer()
{
  if(bit_buff_idx != 7) {
    // Pad remaining bits if needed
    bit_buffer <<= (bit_buff_idx + 1);
    return bit_buffer;
  }
  return NO_RETURN;
}

Arithmetic_Coder::~Arithmetic_Coder()
{
  delete[] occur;
}

std::filesystem::path Arithmetic_Coder::decode(std::ifstream* in_stream)
{
  std::filesystem::path temp_dir = std::filesystem::temp_directory_path();
  std::filesystem::path temp_file_path = temp_dir / "tempfile.txt";

  std::ofstream temp_file(temp_file_path);

  if(!temp_file.is_open()) {
    std::cerr << "Could not create temp file for arithmetic decoding\n";
  }

  uint64_t z = 0;
  uint8_t decoded_sym; // Decoded symbol
  bool eof = false;

  uint8_t byte;
  in_stream->get(reinterpret_cast<char&>(byte));
  uint64_t i = 1;
  while(i <= precision && !eof) {
    if(get_bit(&byte, &bit_buff_idx, in_stream, &eof) != 0)
      z += static_cast<uint64_t>(pow(2, static_cast<double>(precision - i)));
    i++;
  }

  // ------------- MAIN LOOP ----------------

  while(true) {
    uint64_t cumulative_sum = 0;
    for(uint16_t sym = 0; sym < NO_SYMBOLS; sym++) {
      uint64_t dist = u - l;
      cumulative_sum += occur[sym];

      uint64_t new_u =
        l + static_cast<uint64_t>(roundl(static_cast<double>(dist) *
                                         (static_cast<double>(cumulative_sum) /
                                          static_cast<double>(input_size))));
      uint64_t new_l = l + static_cast<uint64_t>(roundl(
                             static_cast<double>(dist) *
                             (static_cast<double>(cumulative_sum - occur[sym]) /
                              static_cast<double>(input_size))));

      if(new_l <= z && z < new_u) {
        decoded_sym = sym;
        input_size++;
        occur[sym]++;
        temp_file.put(static_cast<char>(decoded_sym));
        l = new_l;
        u = new_u;
        break;
      }
    }

    if(eof)
      break;

    while(true) {
      if(u < half) {
      } else if(l > half) {
        l -= half;
        u -= half;
        z -= half;
      } else if(l > quart && u < 3 * quart) {
        l -= quart;
        u -= quart;
        z -= quart;
      } else
        break;

      l <<= 1;
      u <<= 1;
      z <<= 1;
      if(!eof && get_bit(&byte, &bit_buff_idx, in_stream, &eof))
        z += 1;
    }
  }

  temp_file.close();
  return temp_file_path;
}
