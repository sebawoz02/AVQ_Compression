#include <input_reader.hpp>

#include <iostream>
#include <types/header_tga.hpp>

namespace input_reader {

  static std::vector<std::vector<Pixel>> load_image(std::ifstream* input_stream,
                                                    TGA_header tga_head);
  static bool ends_with(const std::string& full_string,
                        const std::string& ending);

  std::vector<std::vector<Pixel>> load_image(std::ifstream* input_stream,
                                             TGA_header tga_head)
  {
    std::vector<std::vector<Pixel>> image(std::vector<std::vector<Pixel>>(tga_head.width,
                                          std::vector<Pixel>(tga_head.height, Pixel(0,0,0))));

    for(int i = 0; i < tga_head.width; i++) {
      for(int j = 0; j < tga_head.height; j++) {
        *input_stream >> std::noskipws >> image[i][j].blue;
        *input_stream >> std::noskipws >> image[i][j].green;
        *input_stream >> std::noskipws >> image[i][j].red;
      }
    }

    return image;
  }

  static bool ends_with(const std::string& full_string,
                        const std::string& ending)
  {
    if(ending.size() > full_string.size())
      return false;

    return full_string.compare(full_string.size() - ending.size(),
                               ending.size(), ending) == 0;
  }

  InReader::InReader(char* input_name)
  {
    std::string filename = std::string(input_name);
    if(ends_with(filename, ".tga")) {
      mode = "tga";
      std::ifstream input_stream = std::ifstream(filename);
      if(!input_stream.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
      }

      // Read header
      TGA_header header{};
      input_stream.read(reinterpret_cast<char*>(&header), sizeof(TGA_header));

      // Load Pixels to 2D vector
      image = load_image(&input_stream, header);
      width = header.width;
      height = header.height;

      input_stream.close();
    } else if(ends_with(filename, ".png")) {
      mode = "png";
    } else {
      mode = "unknown";
      std::cerr << "Cannot read input image. Unknown filetype." << std::endl;
    }
  }

  std::vector<std::vector<Pixel>>& InReader::get_image()
  {
    return image;
  }
} // namespace input_reader
