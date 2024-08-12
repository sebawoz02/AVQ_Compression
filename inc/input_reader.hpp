#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <pixel.hpp>


namespace input_reader {
    typedef struct InReader {
    public:
        explicit InReader(char* input_name);
        std::vector<std::vector<Pixel>>& get_image();
    private:
        std::string mode;
        std::vector<std::vector<Pixel>> image;

    } InReader;
}