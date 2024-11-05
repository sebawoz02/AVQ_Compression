#include <cmath>
#include <fstream>
#include <io_handler.hpp>
#include <iostream>

#define NO_POSSIBLE_SYMBOLS 256

static long double calculate_entropy(const uint32_t* freq_dict,
                                     uint64_t no_symbols);
static void get_freq_table_for_encoded(std::ifstream& stream,
                                       uint64_t* filesize, uint32_t* freq_dict);
static void
get_freq_table_for_original(const char* header, size_t width, size_t height,
                            const std::vector<std::vector<Pixel>>& image,
                            uint32_t* freq_dict);

static void get_freq_table_for_encoded(std::ifstream& stream,
                                       uint64_t* filesize, uint32_t* freq_dict)
{
  char byte;
  while(stream.get(byte)) {
    auto symbol = static_cast<unsigned char>(byte);
    freq_dict[symbol]++;
    (*filesize)++;
  }
}

static void get_freq_table_for_original(
  const char* header, const size_t width, const size_t height,
  const std::vector<std::vector<Pixel>>& image, uint32_t* freq_dict)
{
  char byte;
  for(size_t i = 0; i < TGA_HEADER_SIZE; i++) {
    byte = header[i];
    auto symbol = static_cast<unsigned char>(byte);
    freq_dict[symbol]++;
  }

  for(size_t i = 0; i < width; i++) {
    for(size_t j = 0; j < height; j++) {
      freq_dict[image[i][j].red]++;
      freq_dict[image[i][j].green]++;
      freq_dict[image[i][j].blue]++;
    }
  }
}

static long double calculate_entropy(const uint32_t* freq_dict,
                                     const uint64_t no_symbols)
{
  long double ent =
    0.0; // H = -1 * sum(for(i in freq_dict){ p(i) * log2(p(i)) })
  const auto no_sym = static_cast<long double>(no_symbols);

  for(size_t i = 0; i < NO_POSSIBLE_SYMBOLS; i++) {
    if(freq_dict[i] == 0)
      continue;
    const long double p =
      static_cast<long double>(freq_dict[i]) / no_sym; // p(freq_dict[i])
    ent -= p * log2l(p);
  }
  return ent;
}

int main(int argc, char** argv)
{
  if(argc < 4) {
    std::cerr
      << "Usage: ./analyzer <original_image> <encoded_file> <decoded_image>"
      << std::endl;
  }
  IO_Handler original(argv[1], nullptr);
  original.set_print_summary(false);

  std::ifstream encoded(argv[2]);
  if(!encoded.is_open()) {
    std::cerr << "Cannot open file: " << argv[2] << std::endl;
  }

  IO_Handler decoded(argv[3], nullptr);
  decoded.set_print_summary(false);

  TGA_header og_header{};
  std::vector<std::vector<Pixel>> og_image;

  original.get_header(&og_header);
  original.get_image(og_header.width, og_header.height, &og_image);

  // Compare original with encoded
  // 1. Files size
  // 2. Entropy
  auto* og_freq_dict =
    static_cast<uint32_t*>(calloc(NO_POSSIBLE_SYMBOLS, sizeof(uint32_t)));
  auto* encoded_freq_dict =
    static_cast<uint32_t*>(calloc(NO_POSSIBLE_SYMBOLS, sizeof(uint32_t)));
  uint64_t og_filesize = original.get_bytes_read();
  uint64_t encoded_filesize;

  get_freq_table_for_original(reinterpret_cast<char*>(&og_header),
                              og_header.width, og_header.height, og_image,
                              og_freq_dict);
  get_freq_table_for_encoded(encoded, &encoded_filesize, encoded_freq_dict);

  long double og_entropy = calculate_entropy(og_freq_dict, og_filesize);
  long double encoded_entropy =
    calculate_entropy(encoded_freq_dict, encoded_filesize);

  std::cout << "====== Original vs Encoded ======" << std::endl;
  std::cout << "original filesize: " << og_filesize << " B" << std::endl;
  std::cout << "encoded filesize: " << encoded_filesize << " B" << std::endl;
  std::cout << "original entropy: " << og_entropy << std::endl;
  std::cout << "encoded entropy: " << encoded_entropy << " ("
            << encoded_entropy / og_entropy * 100.0 - 100 << "%)" << std::endl;

  encoded.close();
  free(og_freq_dict);
  free(encoded_freq_dict);

  return 0;
}