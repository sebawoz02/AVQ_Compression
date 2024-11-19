#include <analyzer/stats.hpp>
#include <cmath>
#include <fstream>
#include <io_handler.hpp>
#include <iostream>

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

  original.get_header_tga(&og_header);
  original.get_image_rgb(og_header.width, og_header.height, &og_image);

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
  long double ent_increase = encoded_entropy / og_entropy * 100.0 - 100.0;
  long double size_increase =
    static_cast<long double>(encoded_filesize) / og_filesize * 100.0 - 100.0;

  std::cout << "====== Original vs Encoded ======" << std::endl;
  std::cout << "original filesize: \t" << og_filesize << " B" << std::endl;
  std::cout << "encoded filesize: \t" << encoded_filesize << " B" << " ("
            << ((size_increase >= 0) ? "+" : "") << size_increase << "%)"
            << std::endl;
  std::cout << "original entropy: \t" << og_entropy << std::endl;
  std::cout << "encoded entropy: \t" << encoded_entropy << " ("
            << ((ent_increase >= 0) ? "+" : "") << ent_increase << "%)"
            << std::endl;
  std::cout << std::endl;
  encoded.close();
  free(og_freq_dict);
  free(encoded_freq_dict);

  // Compare original with decoded
  // 1. MSE
  // 2. PSNR
  // 2. SSIM

  TGA_header decoded_header{};
  std::vector<std::vector<Pixel>> decoded_image;

  decoded.get_header_tga(&decoded_header);
  decoded.get_image_rgb(decoded_header.width, decoded_header.height,
                    &decoded_image);

  double mse = calculate_mse(og_image, decoded_image);
  double psnr = calculate_psnr(mse);
  double ssim_r = calculate_ssim_channel(og_image, decoded_image, 'r');
  double ssim_g = calculate_ssim_channel(og_image, decoded_image, 'g');
  double ssim_b = calculate_ssim_channel(og_image, decoded_image, 'b');
  double ssim = calculate_ssim(ssim_r, ssim_g, ssim_b);

  std::cout << "====== Original vs Decoded ======" << std::endl;
  std::cout << "MSE (Mean Square Error): \t\t" << mse << std::endl;
  std::cout << "PSNR (Peak Signal-to-Noise Ratio): \t" << psnr << " dB"
            << std::endl;
  std::cout << "SSIM (Structural Similarity Index):" << std::endl;
  std::cout << "\tred:\t" << ssim_r << std::endl;
  std::cout << "\tgreen:\t" << ssim_g << std::endl;
  std::cout << "\tblue:\t" << ssim_b << std::endl;
  std::cout << "\tavg:\t" << ssim << std::endl;

  return 0;
}