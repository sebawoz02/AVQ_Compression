#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <filesystem>
#include <fstream>


#define NO_RETURN 256

typedef struct Arithmetic_Coder
{
    Arithmetic_Coder();
    std::vector<uint8_t>* encode(uint8_t sym);
    std::filesystem::path decode(std::ifstream* in_stream);
    uint16_t flush_buffer();
    ~Arithmetic_Coder();

private:

    // Encoding
    uint8_t bit_buffer;         // Bit buffer
    size_t bit_buff_idx;        // Index in buffer
    uint64_t l;                 // Lower limit
    uint64_t u;                 // Upper limit
    int16_t scale;              // Scale
    uint64_t* occur;
    uint64_t input_size;
} Arithmetic_Coder;
