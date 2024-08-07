#pragma once

#include <string>
#include <fstream>
#include <types.hpp>


namespace input_reader {
    typedef struct InReader {
    public:
        explicit InReader(char* input_name);
        Image get_image();
    private:
        std::string mode;
        Image image;

    } InReader;
}