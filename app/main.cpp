#include <decoder.hpp>
#include <encoder.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
  if(argc < 4) {
    std::cout << "Usage:\n"
                 "./avq <image_file> <output_file> -e <tolerance>/-d"
              << std::endl;
    return 1;
  }

  IO_Handler io_handler(argv[1], argv[2]);

  std::string mode(argv[3]);

  if(mode == "-e") {
      if(argc < 5)
      {
          std::cout << "Usage:\n"
                       "./avq <image_file> <output_file> -e <tolerance>"
                    << std::endl;
          return 1;
      }
      double tolerance;
      try {
          tolerance = std::stod(argv[4]);
      } catch (const std::invalid_argument& e) {
          std::cerr << "Error: Invalid number format for tolerance." << std::endl;
          return 1;
      } catch (const std::out_of_range& e) {
          std::cerr << "Error: Tolerance value out of range." << std::endl;
          return 1;
      }

    TGA_header header{};
    std::vector<std::vector<Pixel>> image;
    io_handler.get_header(&header);
    io_handler.get_image(header.width, header.height, &image);

    Encoder encoder(&io_handler, heuristic::match::top_left_mse,
                    heuristic::dict_init::range_0_to_255,
                    heuristic::gp_update::first_from_left,
                    heuristic::growing::wave,
                    heuristic::dict_update::one_column_one_row,
                    heuristic::dict_deletion::fifo, tolerance);

    io_handler.write_header(header);
    encoder.encode(image, header.width, header.height);
  } else if(mode == "-d") {
    TGA_header header{};
    std::vector<std::vector<Pixel>> image;
    io_handler.get_header(&header);

    Decoder decoder(&io_handler, heuristic::dict_init::range_0_to_255,
                    heuristic::gp_update::first_from_left,
                    heuristic::growing::wave,
                    heuristic::dict_update::one_column_one_row,
                    heuristic::dict_deletion::fifo);

    image = decoder.decode(header.width, header.height);

    io_handler.write_header(header);
    io_handler.write_image(image, header.width, header.height);
  } else {
    std::cout << "Unknown mode: '" << mode << "'. Expected '-e' or '-d'."
              << std::endl;
    return 1;
  }

  return 0;
}