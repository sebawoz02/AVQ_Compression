#include "arg_parser.hpp"
#include "decoder.hpp"
#include "encoder.hpp"

int main(int argc, char** argv)
{
  Args args = arg_parser::parse(static_cast<size_t>(argc), argv);
  if(args.exit) {
    return 1;
  }
  IO_Handler* io_handler = new IO_Handler(args.in_filename, args.out_filename);

  if(!args.mode) {
    TGA_header header{};
    std::vector<std::vector<Pixel>> image;
    io_handler->get_header(&header);
    io_handler->get_image(header.width, header.height, &image);

    Encoder encoder(io_handler, args.match_heur, args.dict_init_heur,
                    args.growing_point_update_heur, args.growing_heur,
                    args.dict_update_heur, args.deletion_heur, args.tolerance);

    io_handler->write_header(header);
    encoder.encode(image, header.width, header.height);
  } else {
    TGA_header header{};
    std::vector<std::vector<Pixel>> image;
    io_handler->get_header(&header);

    Decoder decoder(io_handler, args.dict_init_heur,
                    args.growing_point_update_heur, args.growing_heur,
                    args.dict_update_heur, args.deletion_heur);

    image = decoder.decode(header.width, header.height);

    io_handler->write_header(header);
    io_handler->write_image(image, header.width, header.height);
  }

  delete io_handler;
  return 0;
}