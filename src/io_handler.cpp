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
    if(ac_mode == AC_ENCODING) {
      out_buffer <<= (8 - out_bit_count);
      auto bytes = arithmetic_coder->encode(out_buffer);
      for(size_t i = 0; i < bytes->size(); i++) {
        out_stream.put(static_cast<char>((*bytes)[i]));
      }
      bytes_wrote += bytes->size();
      delete bytes;
    } else {
      // Pad remaining bits if needed
      out_buffer <<= (8 - out_bit_count);
      out_stream.put(static_cast<char>(out_buffer));
      bytes_wrote++;
    }
    out_buffer = 0;
    out_bit_count = 0;
  }
}

IO_Handler::~IO_Handler()
{
  flush_buffer();
  if(ac_mode == AC_ENCODING) {
    uint16_t byte = arithmetic_coder->flush_buffer();
    if(byte != NO_RETURN) {
      out_stream.put(static_cast<char>(byte));
      bytes_wrote++;
    }
  }

  if(print_summary) {
    std::cout << "IO HANDLER:" << std::endl;
    std::cout << "    in_file size: " << bytes_read << " B" << std::endl;
    std::cout << "    out_file size: " << bytes_wrote << " B" << std::endl;
  }

  in_stream.close();
  out_stream.close();
  delete arithmetic_coder;

  if(ac_mode == AC_DECODING) {
    std::filesystem::remove(temp_path);
  }
}

IO_Handler::IO_Handler(char* in_filename, char* out_filename,
                       Additional_Compression_Mode _ac)
{
  print_summary = true;

  out_bit_count = 0;
  out_buffer = 0;

  in_bit_count = 0;
  in_buffer = 0;

  bytes_read = 0;
  bytes_wrote = 0;

  if(in_filename != nullptr) {
    in_stream = std::ifstream(in_filename);
    if(!in_stream.is_open()) {
      std::cerr << "Cannot open file: " << in_filename << std::endl;
    }
  }

  if(out_filename != nullptr) {
    out_stream = std::ofstream(out_filename);
    if(!out_stream.is_open()) {
      std::cerr << "Cannot open file: " << out_filename << std::endl;
    }
  }
  if(_ac == AC_ENCODING) {
    arithmetic_coder = new Arithmetic_Coder();
  } else if(_ac == AC_DECODING) {
    arithmetic_coder = new Arithmetic_Coder();
    auto path = arithmetic_coder->decode(&in_stream);
    in_stream.close();
    in_stream = std::ifstream(path);
    temp_path = path;
  } else {
    arithmetic_coder = nullptr;
  }
  ac_mode = _ac;
}

void IO_Handler::get_image(size_t width, size_t height,
                           std::vector<std::vector<Pixel>>* image)
{
  std::vector<std::vector<Pixel>> _image(std::vector<std::vector<Pixel>>(
    width, std::vector<Pixel>(height, Pixel(0, 0, 0))));

  for(size_t i = 0; i < width; i++) {
    for(size_t j = 0; j < height; j++) {
      in_stream.read(reinterpret_cast<char*>(&_image[i][j]), 3);
      bytes_read += 3;
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
      bytes_read++;
    }

    bool bit = (in_buffer >> (in_bit_count - 1)) & 1;
    result = (result << 1) | bit;
    --in_bit_count;
  }
  return result;
}

void IO_Handler::write_image(const std::vector<std::vector<Pixel>>& image,
                             const size_t width, const size_t height)
{
  for(size_t i = 0; i < width; i++) {
    for(size_t j = 0; j < height; j++) {
      out_stream.write(reinterpret_cast<const char*>(&image[i][j]),
                       sizeof(Pixel));
      bytes_wrote += 3;
    }
  }
}

void IO_Handler::get_header(TGA_header* header)
{
  in_stream.read(reinterpret_cast<char*>(header), sizeof(TGA_header));
  bytes_read += sizeof(TGA_header);
}

void IO_Handler::write_header(const TGA_header& header)
{
  out_stream.write(reinterpret_cast<const char*>(&header), sizeof(TGA_header));
  bytes_wrote += sizeof(TGA_header);
}

uint64_t IO_Handler::get_bytes_read() const
{
  return bytes_read;
}

void IO_Handler::set_print_summary(bool ps)
{
  print_summary = ps;
}
