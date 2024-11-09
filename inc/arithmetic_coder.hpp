#pragma once

#include <filesystem>

typedef struct Arithmetic_Coder
{
    explicit Arithmetic_Coder(std::ifstream* _in_stream) : in_stream(_in_stream){};
    uint64_t encode(std::ofstream* out_stream);
    std::filesystem::path decode();

private:
    std::ifstream* in_stream;
} Arithmetic_Coder;
