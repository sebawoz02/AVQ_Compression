#pragma once

#include <fstream>
#include <cstdint>

namespace output_writer {
    typedef struct OutWriter
    {
        explicit OutWriter(std::ofstream& out) : out_stream(out), bit_idx(7), bit_buffer(0){};

        void write(bool bit);
        void flush_buffer();

        ~OutWriter();
    private:
        std::ofstream& out_stream;
        size_t bit_idx;
        uint8_t bit_buffer;
    } OutWriter;
}