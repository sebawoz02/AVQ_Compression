#include <encoder.hpp>
#include <input_reader.hpp>

#include <iostream>
#include <fstream>
#include <vector>


int main(int argc, char** argv)
{
    if(argc < 3) {
        std::cout << "Usage:\n"
                     "./avq <image_file> <output_file>"
                  << std::endl;
        return 1;
    }

    input_reader::InReader inreader(argv[1]);

    std::vector<std::vector<Pixel>> image = inreader.get_image();

    std::ofstream out(argv[2]);

    encoder::Encoder encoder1(&out,
                     heuristic::match::top_left_mse,
                     heuristic::dict_init::range_0_to_255,
                     heuristic::gp_update::first_from_left,
                     heuristic::growing::wave,
                     heuristic::dict_update::one_column_one_row,
                     heuristic::dict_deletion::fifo,
                     0.99);

    encoder1.encode(image, inreader.width, inreader.height);

    out.close();

    return 0;
}