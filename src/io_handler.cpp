#include <io_handler.hpp>
#include <iostream>

void IO_Handler::write(bool bit)
{
  out_buffer = (out_buffer << 1) | bit;
  ++out_bit_count;

  if(out_bit_count == 8) {
    flush_buffer();
  }
}

void IO_Handler::flush_buffer()
{
  if(out_bit_count > 0) {
    // Pad remaining bits if needed
    out_buffer <<= (8 - out_bit_count);
    out_stream.put(static_cast<char>(out_buffer));
    out_buffer = 0;
    out_bit_count = 0;
  }
}

IO_Handler::~IO_Handler()
{
  flush_buffer();
  in_stream.close();
  out_stream.close();
}

IO_Handler::IO_Handler(char* in_filename, char* out_filename)
{
  out_bit_count = 0;
  out_buffer = 0;

  in_bit_count = 0;
  in_buffer = 0;

  in_stream = std::ifstream(in_filename);
  if(!in_stream.is_open()) {
    std::cerr << "Cannot open file: " << in_filename << std::endl;
  }

  out_stream = std::ofstream(out_filename);
  if(!out_stream.is_open()) {
    std::cerr << "Cannot open file: " << out_filename << std::endl;
  }
}

void IO_Handler::get_image(size_t width, size_t height,
                           std::vector<std::vector<Pixel>>* image)
{
  std::vector<std::vector<Pixel>> _image(std::vector<std::vector<Pixel>>(
    width, std::vector<Pixel>(height, Pixel(0, 0, 0))));

  for(size_t i = 0; i < width; i++) {
    for(size_t j = 0; j < height; j++) {
      in_stream >> std::noskipws >> _image[i][j].blue;
      in_stream >> std::noskipws >> _image[i][j].green;
      in_stream >> std::noskipws >> _image[i][j].red;
    }
  }
  *image = _image;
}

uint16_t IO_Handler::read_bits(size_t x)
{
  uint16_t result = 0;
  for(size_t i = 0; i < x; ++i) {
    if(in_bit_count == 0) {
      if(!in_stream.get(reinterpret_cast<char&>(in_buffer))) {
        throw std::runtime_error("End of file reached");
      }
      in_bit_count = 8;
    }

    bool bit = (in_buffer >> (in_bit_count - 1)) & 1;
    result = (result << 1) | bit;
    --in_bit_count;
  }
  return result;
}

void IO_Handler::write_image(const std::vector<std::vector<Pixel>>& image) {
    (void) image;
}

void IO_Handler::get_header(TGA_header* header)
{
  in_stream.read(reinterpret_cast<char*>(header), sizeof(TGA_header));
}

void IO_Handler::write_header(const TGA_header &header) {
  out_stream.write(reinterpret_cast<const char*>(&header), sizeof(TGA_header));
}
