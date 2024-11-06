#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <types/pixel.hpp>
#include <vector>

#define NO_POSSIBLE_SYMBOLS 256

long double calculate_entropy(const uint32_t* freq_dict, uint64_t no_symbols);
double calculate_mse(const std::vector<std::vector<Pixel>>& img1,
                     const std::vector<std::vector<Pixel>>& img2);
double calculate_psnr(double mse);
double calculate_ssim(double ssim_r, double ssim_g, double ssim_b);
double calculate_ssim_channel(const std::vector<std::vector<Pixel>>& img1,
                              const std::vector<std::vector<Pixel>>& img2,
                              char channel, size_t window_size = 8);
