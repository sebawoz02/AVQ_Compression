#include <arg_parser.hpp>
#include <decoder.hpp>
#include <encoder.hpp>
#include <iostream>

int main(int argc, char** argv)
{
  Args args = arg_parser::parse(static_cast<size_t>(argc), argv);
  if(args.exit) {
    return 1;
  }
  auto* io_handler = new IO_Handler(args.in_filename, args.out_filename);

  if(!args.mode) {
    if(io_handler->input_format == FORMAT_UNKNOWN) {
      std::cerr << "Input file format not supported: " << args.in_filename
                << std::endl;
      delete io_handler;
      return 1;
    }

    if(io_handler->input_format == FORMAT_TGA) {
      TGA_header header{};
      std::vector<std::vector<Pixel>> image;
      io_handler->get_header_tga(&header);
      io_handler->get_image_rgb(header.width, header.height, &image);
      const uint16_t width = header.width;
      const uint16_t height = header.height;

      std::vector<std::vector<uint8_t>> red(width,
                                            std::vector<uint8_t>(height, 0));
      std::vector<std::vector<uint8_t>> green(width,
                                              std::vector<uint8_t>(height, 0));
      std::vector<std::vector<uint8_t>> blue(width,
                                             std::vector<uint8_t>(height, 0));

      for(size_t i = 0; i < width; i++) {
        for(size_t j = 0; j < height; j++) {
          red[i][j] = image[i][j].red;
          green[i][j] = image[i][j].green;
          blue[i][j] = image[i][j].blue;
        }
      }

      Encoder encoder(io_handler, args.match_heur, args.dict_init_heur,
                      args.growing_point_update_heur, args.growing_heur,
                      args.dict_update_heur, args.deletion_heur,
                      args.tolerance);

      io_handler->write_header_tga(header);
      encoder.encode(Image(red, width, height));
      encoder.encode(Image(green, width, height));
      encoder.encode(Image(blue, width, height));
    }
  } else {
    if(io_handler->output_format == FORMAT_UNKNOWN) {
      std::cerr << "Unknown output file format: " << args.out_filename
                << std::endl;
      delete io_handler;
      return 1;
    }

    if(io_handler->output_format == FORMAT_TGA) {
      TGA_header header{};
      std::vector<std::vector<Pixel>> image;
      io_handler->get_header_tga(&header);

      Decoder decoder(io_handler, args.dict_init_heur,
                      args.growing_point_update_heur, args.growing_heur,
                      args.dict_update_heur, args.deletion_heur);

      image = decoder.decode(header.width, header.height);

      io_handler->write_header_tga(header);
      io_handler->write_image_rgb(image, header.width, header.height);
    }
  }

  delete io_handler;
  return 0;
}