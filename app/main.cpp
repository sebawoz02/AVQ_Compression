#include <encoder.hpp>
#include <input_reader.hpp>

#include <iostream>
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

    // encoder::Encoder();

    return 0;
}